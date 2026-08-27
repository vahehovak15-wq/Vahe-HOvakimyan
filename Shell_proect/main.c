#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "parser.h"
#include "executor.h"
#include <unistd.h>
#include <ctype.h>
#define MAX_HISTORY 100

void sigint_handler(int sig) {
    printf("\n");
}
int handle_command (char **args){
    if (args[0] != NULL && strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                printf("cd: missing argument\n");
            } else {  
                if (chdir(args[1]) != 0) {
                    perror("cd");
                }
            }
            
        return 1;
        }
        if (args[0] != NULL && strcmp(args[0], "help") == 0) {
                printf("Available commands:\n");
                printf("  cd [dir]        - change directory\n");
                printf("  pwd             - print current directory\n");
                printf("  echo [text]     - print text\n");
                printf("  history         - show command history\n");
                printf("  clear           - clear screen\n");
                printf("  setenv          - set environment variable\n");
                printf("  unsetenv        - unset environment variable\n");
                printf("  chprompt        - change shell prompt\n");
                printf("  exit            - exit shell\n");
                return 1;
            }
            
        if (args[0] != NULL && strcmp(args[0], "pwd") == 0) {
             char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("cwd = %s\n", cwd);
            } else {
                perror("pwd");
            }

            return 1;
        }

        if (args[0] != NULL && strcmp(args[0], "echo") == 0){
            for (int i = 1; args[i] != NULL; i++) {
                if (args[i][0] == '$') {
                    char *value = getenv(args[i] + 1);

                    if (value != NULL) {
                        printf("%s ", value);
                    }
                } else {
                    printf("%s ", args[i]);
                }
            }
            printf("\n");
            return 1;
        }

        if (args[0] != NULL && strcmp(args[0], "clear") == 0){
            printf("\033[2J\033[H");
            return 1;
        }
        if (args[0] != NULL && strcmp(args[0], "setenv") == 0){
            if(args[1] == NULL || args[2] == NULL){
                printf("setenv.missing argument\n");
            }else{
                if(setenv(args[1],args[2],1) == -1){
                    perror("setenv");
                }
            }
            return 1;
        }
        if (args[0] != NULL && strcmp(args[0], "unsetenv") == 0){
            if(args[1] == NULL ){
                printf("unsetenv.missing argument\n");
            }else{
                if(unsetenv(args[1]) == -1){
                    perror("unsetenv");
                }
            }
            return 1;
        }
        return 0;
}
int main() {

    signal (SIGINT,sigint_handler);
    char* line = NULL;
    size_t bufsize = 0;

    char *history[MAX_HISTORY];
    int history_count = 0;

    char prompt[100] = "my_shell";
    while (1) {
        printf("%s>",prompt);
        fflush(stdout);
        ssize_t characters_read = getline(&line, &bufsize, stdin);
        
        if (characters_read == -1) {
            
            printf("\nExiting shell...\n");
            break;
        }
        
        if(history_count < MAX_HISTORY){
            history[history_count] = strdup(line);
            history_count++;
        }
        char** args = parse_line(line);
        if (args[0] != NULL && args[0][0] == '!') {
            int valid = 1;

            for (int i = 1; args[0][i] != '\0'; i++) {
                if (!isdigit(args[0][i])) {
                    valid = 0;
                    break;
                }
            }

            if (!valid || args[0][1] == '\0') {
                printf("Invalid history number\n");
                free(args);
                continue;
            }
        int number = atoi(args[0] + 1);

        if (number < 1 || number > history_count) {
            printf("Invalid history number\n");
        } else {
            char *cmd = strdup(history[number - 1]);
            char **new_args = parse_line(cmd);

            int new_handled = handle_command(new_args);

            if (new_handled == 0) {
                execute_cmd(new_args);
            } else {
                free(new_args);
            }

            free(cmd);
            }

            free(args);
            continue;
        }
        
        if (args[0] != NULL && strcmp(args[0], "history") == 0) {
            for(int i = 0;i < history_count;i++){
                printf("%d %s",i,history[i]);
            }
            free(args);
            continue;
        }
        if (args[0] != NULL && strcmp(args[0], "exit") == 0) {
            free(args);
            break;
        }
        

        if (args[0] != NULL && strcmp(args[0], "chprompt") == 0) {
            if(args[1] == NULL){
                printf("chprompt.missing argument\n");
            }else{
                strcpy(prompt, args[1]);
            }
            free(args);
            continue;
        }
        int handled = handle_command(args);

        if (handled == 0) {
            execute_cmd(args);
        } else {
            free(args);
        }
    }
    for (int i = 0; i < history_count; i++) {
        free(history[i]);
    }
    free(line); 
    return 0;
}