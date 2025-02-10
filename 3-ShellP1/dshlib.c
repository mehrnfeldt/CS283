#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
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

    char *saveptr1, *saveptr2;
    char *cmd_str = strtok_r(cmd_line, PIPE_STRING, &saveptr1);

    while (cmd_str != NULL) {
        if (clist->num >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;  
        }

        while (*cmd_str == SPACE_CHAR) {
            cmd_str++;
        }

        len = strlen(cmd_str);
        while (len > 0 && cmd_str[len - 1] == SPACE_CHAR) {
            cmd_str[--len] = '\0';
        }

        command_t *cmd = &clist->commands[clist->num];

        char *token = strtok_r(cmd_str, " ", &saveptr2);
        if (!token || strlen(token) >= EXE_MAX) {
            return ERR_CMD_OR_ARGS_TOO_BIG; 
        }
        strcpy(cmd->exe, token);
        cmd->args[0] = '\0';

        token = strtok_r(NULL, " ", &saveptr2);  
        while (token != NULL) {
            if (strlen(token) >= ARG_MAX) {
                return ERR_CMD_OR_ARGS_TOO_BIG;  
            }
            
            if (cmd->args[0] == '\0') {
                strcpy(cmd->args, token);
            } else {
                strcat(cmd->args, " ");
                strcat(cmd->args, token);
            }
            token = strtok_r(NULL, " ", &saveptr2);
        }
        
        clist->num++;

        cmd_str = strtok_r(NULL, PIPE_STRING, &saveptr1);
    }

    return OK;     
    
    }
