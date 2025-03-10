#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>

#include "dshlib.h"


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>

#include "dshlib.h"

/**** 
 **** FOR REMOTE SHELL USE YOUR SOLUTION FROM SHELL PART 3 HERE
 **** THE MAIN FUNCTION CALLS THIS ONE AS ITS ENTRY POINT TO
 **** EXECUTE THE SHELL LOCALLY
 ****
 */

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int last_return_code = 0;
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    if (!cmd_line || !clist) {
        return ERR_CMD_OR_ARGS_TOO_BIG;
    }

    memset(clist, 0, sizeof(command_list_t));

    while (*cmd_line == SPACE_CHAR) {
        cmd_line++;
    }

    int len = strlen(cmd_line);
    while (len > 0 && (cmd_line[len - 1] == SPACE_CHAR || cmd_line[len - 1] == '\n')) {
        cmd_line[--len] = '\0';
    }

    if (len == 0) {
        return WARN_NO_CMDS;
    }

    char *saveptr1;
    char *cmd_str = strtok_r(cmd_line, PIPE_STRING, &saveptr1);

    while (cmd_str != NULL) {
        if (clist->num >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }

        cmd_buff_t *cmd = &clist->commands[clist->num];
        cmd->argc = 0;
	cmd->input_file = NULL;
        cmd->output_file = NULL;

        char *token;
        char *saveptr2;
        token = strtok_r(cmd_str, " ", &saveptr2);
    while (token != NULL) {
        if (strcmp(token, "<") == 0) {
            token = strtok_r(NULL, " ", &saveptr2);
            if (!token) return ERR_CMD_OR_ARGS_TOO_BIG;
            cmd->input_file = token;
        } else if (strcmp(token, ">>") == 0) {  
            token = strtok_r(NULL, " ", &saveptr2);
            if (!token) return ERR_CMD_OR_ARGS_TOO_BIG;
            cmd->output_file = token;
            cmd->append_mode = true;  
        } else if (strcmp(token, ">") == 0) {  
            token = strtok_r(NULL, " ", &saveptr2);
            if (!token) return ERR_CMD_OR_ARGS_TOO_BIG;
            cmd->output_file = token;
            cmd->append_mode = false; 
        } else {
            if (cmd->argc >= CMD_ARGV_MAX - 1) return ERR_CMD_OR_ARGS_TOO_BIG;
            cmd->argv[cmd->argc++] = token;
        }
	    token = strtok_r(NULL, " ", &saveptr2);
    }
        cmd->argv[cmd->argc] = NULL;

        clist->num++;
        cmd_str = strtok_r(NULL, PIPE_STRING, &saveptr1);
    }

    return OK;
}

int exec_local_cmd_loop()
{
    char cmd_buff[SH_CMD_MAX];
    int rc = 0;
    command_list_t clist;

    while (1) {
        printf("%s", SH_PROMPT);

        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }

        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        if (strcmp(cmd_buff, "") == 0) {
            printf("%s", CMD_WARN_NO_CMD);
            last_return_code = WARN_NO_CMDS;
            continue;
        }

        memset(&clist, 0, sizeof(command_list_t));

        rc = build_cmd_list(cmd_buff, &clist);

        if (rc != OK) {
            if (rc == ERR_TOO_MANY_COMMANDS) {
                printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            } else {
                fprintf(stderr, "Error: Failed to parse command line\n");
            }
            last_return_code = rc;
            continue;
        }

        if (clist.num == 1) {
            cmd_buff_t *first_cmd = &clist.commands[0];

            if (strcmp(first_cmd->argv[0], EXIT_CMD) == 0) {
                exit(OK_EXIT);
            } else if (strcmp(first_cmd->argv[0], "cd") == 0) {
                if (first_cmd->argc > 1) {
                    if (chdir(first_cmd->argv[1]) != 0) {
                        perror("cd failed");
                        last_return_code = errno;
                    } else {
                        last_return_code = OK;
                    }
                }
                continue;
            } else if (strcmp(first_cmd->argv[0], "rc") == 0) {
                printf("%d\n", last_return_code);
                continue;
            } else if (strcmp(first_cmd->argv[0], "dragon") == 0) {
                print_dragon();
                last_return_code = OK;
                continue;
            }
        }

        int pipefds[2 * (clist.num - 1)];
        for (int i = 0; i < clist.num - 1; i++) {
            if (pipe(pipefds + i * 2) < 0) {
                perror("pipe");
                return errno;
            }
	    
        }

        for (int i = 0; i < clist.num; i++) {
	    pid_t pid = fork();
            if (pid == 0) {
        	if (clist.commands[i].input_file) {
            		int in_fd = open(clist.commands[i].input_file, O_RDONLY);
            		if (in_fd < 0) {
                		perror("Input redirection failed");
                		exit(errno);
            		}
            		dup2(in_fd, STDIN_FILENO);
           		close(in_fd);
        	}

        	if (clist.commands[i].output_file) {
		    int out_fd;
    			if (clist.commands[i].append_mode) {
        			out_fd = open(clist.commands[i].output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    			} else {
        			out_fd = open(clist.commands[i].output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    			}	
    
   		if (out_fd < 0) {
        		perror("open output");
        		exit(errno);
   		 }
    		dup2(out_fd, STDOUT_FILENO);
    		close(out_fd);
		}


                if (i > 0) {
                    if (dup2(pipefds[(i - 1) * 2], STDIN_FILENO) < 0) {
                        perror("dup2 input");
                        exit(errno);
                    }
                }

                if (i < clist.num - 1) {
                    if (dup2(pipefds[i * 2 + 1], STDOUT_FILENO) < 0) {
                        perror("dup2 output");
                        exit(errno);
                    }
                }

                for (int j = 0; j < 2 * (clist.num - 1); j++) {
                    close(pipefds[j]);
             }
		execvp(clist.commands[i].argv[0], clist.commands[i].argv);
                perror("execvp");
                exit(errno);
            } else if (pid < 0) {
                perror("fork");
                return errno;
            }
        }

        for (int i = 0; i < 2 * (clist.num - 1); i++) {
            close(pipefds[i]);
        }

        for (int i = 0; i < clist.num; i++) {
            int status;
            wait(&status);
            if (WIFEXITED(status)) {
                last_return_code = WEXITSTATUS(status);
            } else {
                last_return_code = -1;
            }
        }
    }

    return OK;
} 
