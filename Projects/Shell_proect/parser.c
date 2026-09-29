#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "parser.h"
#include <string.h>

#define INITIAL_BUFSIZE 16
#define DELIM " \n\a\r\t"

char** parse_line(char* line){
    int bufsize = INITIAL_BUFSIZE;
    int position = 0;
    char** tokens = malloc(bufsize * sizeof(char*));
    if (!tokens) {
        perror("Allocation error");
        exit(EXIT_FAILURE);
    }
    char* token = strtok(line, DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += INITIAL_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                perror("Reallocation error");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, DELIM);
    }
    tokens[position] = NULL;
    return tokens;

}