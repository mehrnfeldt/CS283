#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"
#include <errno.h>

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

int exec_local_cmd_loop()
{
    char cmd_buff[SH_CMD_MAX];
    int rc = 0;
    cmd_buff_t cmd;

    // TODO IMPLEMENT MAIN LOOP

    // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff

    // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
    // the cd command should chdir to the provided directory; if no directory is provided, do nothing

    // TODO IMPLEMENT if not built-in command, fork/exec as an external command
    // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"

    while (1) {
	printf("%s", SH_PROMPT);

        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;  
        }

        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';
        memset(&cmd, 0, sizeof(cmd_buff_t)); 

	cmd.argc = 0;
	char *ptr = cmd_buff;
        int in_quotes = 0;

        while (*ptr) {
            while (*ptr == ' ') ptr++;
            if (*ptr == '\0') break;  
            if (*ptr == '"') {
                ptr++; 
                cmd.argv[cmd.argc++] = ptr; 
                in_quotes = 1;
                while (*ptr && !(*ptr == '"' && *(ptr - 1) != '\\')) ptr++; 
                if (*ptr == '"') {
                    *ptr = '\0'; 
                    ptr++;
                }
            } else {
                cmd.argv[cmd.argc++] = ptr;
                while (*ptr && *ptr != ' ') ptr++; 
                if (*ptr == ' ') {
                    *ptr = '\0';
                    ptr++; 
                }
            }
            if (cmd.argc >= CMD_ARGV_MAX - 1) break; 
        }

        cmd.argv[cmd.argc] = NULL; 	
	
	if (cmd.argc > 0) {
            if (strcmp(cmd.argv[0], EXIT_CMD) == 0) {
		exit(OK_EXIT);
		break;  
            } else if (strcmp(cmd.argv[0], "cd") == 0) {
		    
		if (cmd.argc > 1) {
                    if (chdir(cmd.argv[1]) != 0 ){
			    perror("cd failed");
			    last_return_code = errno;
		    }else{
			    last_return_code = OK;
		    }

		}
		continue;
	    }else if (strcmp(cmd.argv[0], "rc") == 0){
		    printf("%d\n", last_return_code);
		    continue;
	    }
	}

	if (strcmp(cmd_buff, "dragon") == 0) {
            print_dragon();
	    last_return_code = OK;
            continue;  
        }
	if (strcmp(cmd_buff, "") == 0) {
            printf("%s", CMD_WARN_NO_CMD);
	    last_return_code= WARN_NO_CMDS; 
            continue;  
        }	
	
        pid_t pid = fork();
        if (pid == 0) {
	    execvp(cmd.argv[0], cmd.argv);
	    switch (errno){
		    case ENOENT:
			    fprintf(stderr, "Error: Command not found\n");
			    break;
		    case EACCES:
			    fprintf(stderr, "Error: Permission denied\n");
			    break;
		    default:
			    fprintf(stderr, "Error: Execution failed\n");
			    break;
	    }
	    exit(errno);
	}else if (pid > 0){
		int status;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status)){
			last_return_code = WEXITSTATUS(status);
		} else {
		       last_return_code = -1;
		}
	} else {
		perror("Fork failed");
		last_return_code = errno;
	}
    }	
	

    return OK;
}



