#include <stdio.h>
#include "input.h"

int main() {

    char line[BUFSIZ];
    char **tokens = malloc(sizeof(char*));
    size_t n = 3;
    prompt_for_tokens(line, tokens, n);

    for (size_t i = 0; i < n; ++i) {
        printf("%s ", tokens[i]);
    }
}

