


#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

int main() {
    char line[BUFSIZ];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        perror("Could not read from stdin!");
        exit(EXIT_FAILURE);
    }
    line[strlen(line) - 1] = '\0';

    printf("%s", line);

    for (size_t i = 0; i < strlen(line); ++i) {
        printf("%x ", line[i]);
    }
}