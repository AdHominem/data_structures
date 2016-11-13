#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>


/// Wraps fgets to handle errors and safely replaces the newline
/// \param line - The buffer which holds the string
/// \param length - The size of the buffer
void get_line(char *line, const size_t length) {
    if (fgets(line, length, stdin)) {
        char *newline = strchr(line, '\n');
        if (newline) {
            *newline = '\0';
        }
    } else {
        fprintf(stderr, "Could not read line from stdin!");
        exit(EXIT_FAILURE);
    }
}


/// Tokenizes a line into the specified number of tokens using the " " delimiter. Excess tokens are omitted. This
/// function will alter the line parameter, so be sure to pass a copy if you want to preserve it.
/// \param line - The string to be processed
/// \param tokens - The buffer holding the tokens
/// \return EXIT_FAILURE if the supplied line does not contain enough tokens.
int get_tokens(char *line, char **tokens, const size_t number_of_tokens) {
    for (size_t i = 0; i < number_of_tokens; ++i) {
        char *token = strtok(line, " ");
        if (token) {
            tokens[i] = token;
            line = NULL;
        } else {
            // If token is NULL that means not enough tokens found
            fprintf(stderr, "Not enough tokens!");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}


/// Not using atoi() since it can't detect errors (ERR34-C)
/// \param string - The string to be interpreted
/// \param integer - A pointer to the returned int value
/// \return EXIT_FAILURE if the string value is out of range or some error occurs
int string_to_integer(char *string, int *integer) {
    char *p;
    const long value = strtol(string, &p, 10);

    if (p == string) {
        fprintf(stderr, "%s: not a decimal number\n", string);
    } else if (*p != '\0') {
        fprintf(stderr, "%s: extra characters at end of input: %s\n", string, p);
    } else if ((LONG_MIN == value || LONG_MAX == value) && ERANGE == errno) {
        fprintf(stderr, "%s out of range of type long\n", string);
    } else if (value > INT_MAX) {
        fprintf(stderr, "%ld greater than INT_MAX\n", value);
    } else if (value < INT_MIN) {
        fprintf(stderr, "%ld less than INT_MIN\n", value);
    } else {
        *integer = (int) value;
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

/// \param string - The string to be interpreted
/// \param size_t - A pointer to the returned size_t value
/// \return EXIT_FAILURE if the string value is out of range or some error occurs
int string_to_size_t(char *string, size_t *size) {
    char *p;
    const long value = strtol(string, &p, 10);

    if (p == string) {
        fprintf(stderr, "%s: not a decimal number\n", string);
    } else if (*p != '\0') {
        fprintf(stderr, "%s: extra characters at end of input: %s\n", string, p);
    } else if ((LONG_MIN == value || LONG_MAX == value) && ERANGE == errno) {
        fprintf(stderr, "%s out of range of type long\n", string);
    } else if (value < 0) {
        fprintf(stderr, "%ld less than 0\n", value);
    } else if ((size_t) value > SIZE_MAX) {
        fprintf(stderr, "%ld greater than SIZE_MAX\n", value);
    } else {
        printf ("A valid size_t\n");
        *size = (size_t) value;
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int get_integer(int *value) {
    char buffer[BUFSIZ];
    get_line(buffer, BUFSIZ);

    char **tokens = malloc(sizeof(char*));
    if (get_tokens(buffer, tokens, 1)) {
        return EXIT_FAILURE;
    }

    int result;
    if (string_to_integer(tokens[0], &result)) {
        return EXIT_FAILURE;
    }

    *value = result;
    return EXIT_SUCCESS;
}

int get_size_t(size_t *value) {
    char buffer[BUFSIZ];
    get_line(buffer, BUFSIZ);

    char **tokens = malloc(sizeof(char*));
    if (get_tokens(buffer, tokens, 1)) {
        return EXIT_FAILURE;
    }

    size_t result;
    if (string_to_size_t(tokens[0], &result)) {
        return EXIT_FAILURE;
    }

    *value = result;
    return EXIT_SUCCESS;
}

// TODO Build prompt loops