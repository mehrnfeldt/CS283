#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

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

        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';  // Remove trailing newline

        memset(&cmd, 0, sizeof(cmd_buff_t)); // Initialize struct

	cmd.argc = 0;
	char *ptr = cmd_buff;
        int in_quotes = 0;

        while (*ptr) {
            // Skip leading spaces
            while (*ptr == ' ') ptr++;

            if (*ptr == '\0') break;  // End of input

            // Handle quoted strings
            if (*ptr == '"') {
                ptr++; // Skip opening quote
                cmd.argv[cmd.argc++] = ptr; // Start of quoted argument
                in_quotes = 1;
                
                while (*ptr && !(*ptr == '"' && *(ptr - 1) != '\\')) ptr++; // Find closing quote

                if (*ptr == '"') {
                    *ptr = '\0'; // Null terminate the quoted argument
                    ptr++; // Move past closing quote
                }
            } else {
                // Regular argument (not quoted)
                cmd.argv[cmd.argc++] = ptr;
                while (*ptr && *ptr != ' ') ptr++; // Find next space
                if (*ptr == ' ') {
                    *ptr = '\0'; // Null terminate argument
                    ptr++; // Move to next argument
                }
            }

            if (cmd.argc >= CMD_ARGV_MAX - 1) break; // Prevent overflow
        }

        cmd.argv[cmd.argc] = NULL; 	
	
	if (cmd.argc > 0) {
            if (strcmp(cmd.argv[0], EXIT_CMD) == 0) {
		exit(0);
		break;  // Exit shell loop
            } else if (strcmp(cmd.argv[0], "cd") == 0) {
		    
		if (cmd.argc > 1) {
                    if (chdir(cmd.argv[1]) != 0 ){
			    perror("cd failed");
		    }

		}
		continue;
	    }
	}

	if (strcmp(cmd_buff, "dragon") == 0) {
            print_dragon();  // Call the function to print the dragon
            continue;  // Go back to the prompt
        }
	if (strcmp(cmd_buff, "") == 0) {
            printf("%s\n", CMD_WARN_NO_CMD);
            continue;  
        }	
	
	// If not a built-in command, fork and execute external command
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            if (execvp(cmd.argv[0], cmd.argv) == -1) {
                perror("Execution failed");
                exit(ERR_EXEC_CMD);
            }
        } else if (pid > 0) {
            // Parent process waits
            wait(NULL);
        } else {
            perror("Fork failed");
        }
    }

    return OK;
}



