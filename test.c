#include <stdio.h>
#include "input.h"

int main() {

    // this will also be the buffer length for the tokens
    const size_t line_length = 10;
    char line[line_length];
    get_line(line, line_length);

    const size_t tokens_wanted = 2;
    char **tokens = malloc(tokens_wanted * sizeof(char*));
    if (tokens == NULL) {
        perror("Could not allocate memory!");
        return EXIT_FAILURE;
    }

    if (get_tokens(line, tokens, tokens_wanted)) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < tokens_wanted; ++i) {
        printf("Found token: %s\n", tokens[i]);
        int value;
        get_integer(tokens[i], &value);
    }
    free(tokens);
}

