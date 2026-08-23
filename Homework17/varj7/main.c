#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "executor.h"

int main() {
    char* line = NULL;
    size_t bufsize = 0;

    while (1) {
        printf("my_shell> ");
        fflush(stdout);
        ssize_t characters_read = getline(&line, &bufsize, stdin);
        
        if (characters_read == -1) {
            
            printf("\nExiting shell...\n");
            break;
        }
        char** args = parse_line(line);

        if (args[0] != NULL && strcmp(args[0], "exit") == 0) {
            free(args);
            break;
        }
        execute_cmd(args);
    }

    free(line); 
    return 0;
}