#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>


/// Wraps fgets to handle errors and safely replaces the newline
/// \param line - The buffer which holds the string
/// \param length - The size of the buffer
void get_line(char *line, const int length) {
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


/// Tokenizes a line into EXACTLY number_of_tokens tokens using the " " delimiter. Excess tokens are omitted. This
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
            //fprintf(stderr, "Not enough tokens!\n");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}


/// Tokenizes a line into UP TO the specified number of tokens using the " " delimiter. Excess tokens are omitted. This
/// function will alter the line parameter, so be sure to pass a copy if you want to preserve it.
/// You will have to check if each token is NULL before accessing them!
/// \param line - The string to be processed
/// \param tokens - The buffer holding the tokens
void get_tokens_optional(char *line, char **tokens, const size_t number_of_tokens) {
    for (size_t i = 0; i < number_of_tokens; ++i) {
        char *token = strtok(line, " ");
        tokens[i] = token;
        line = NULL;
    }
}


/// Not using atoi() since it can't detect errors (ERR34-C)
/// \param string - The string to be interpreted
/// \param integer - A pointer to the returned int value
/// \return EXIT_FAILURE if the string value is out of range or some error occurs
int string_to_integer(char *string, int *integer) {
    char *p;
    const long value = strtol(string, &p, 10);

    if (p == string) {
        fprintf(stderr, "%s is not a decimal number\n", string);
    } else if (*p != '\0') {
        fprintf(stderr, "%s has extra characters at end of input: %s\n", string, p);
    } else if ((LONG_MIN == value || LONG_MAX == value) && ERANGE == errno) {
        fprintf(stderr, "%s is out of range of type int\n", string);
    } else if (value > INT_MAX) {
        fprintf(stderr, "%ld is greater than INT_MAX\n", value);
    } else if (value < INT_MIN) {
        fprintf(stderr, "%ld is less than INT_MIN\n", value);
    } else {
        *integer = (int) value;
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}


/// Converts a string to a size_t
/// \param string - The string to be interpreted
/// \param size_t - A pointer to the returned size_t value
/// \return EXIT_FAILURE if the string value is out of range or some error occurs
int string_to_size_t(char *string, size_t *size) {
    char *p;
    const long value = strtol(string, &p, 10);

    if (p == string) {
        fprintf(stderr, "%s is not a decimal number\n", string);
    } else if (*p != '\0') {
        fprintf(stderr, "%s has extra characters at end of input: %s\n", string, p);
    } else if ((LONG_MIN == value || LONG_MAX == value) && ERANGE == errno) {
        fprintf(stderr, "%s is out of range of type size_t\n", string);
    } else if (value < 0) {
        fprintf(stderr, "%ld is less than 0\n", value);
    } else if ((size_t) value > SIZE_MAX) {
        fprintf(stderr, "%ld is greater than SIZE_MAX\n", value);
    } else {
        *size = (size_t) value;
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}


/// Prompts for an integer value once
/// \param value - A pointer to the integer
/// \return EXIT_FAILURE if the supplied input doesn't match
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
    free(tokens);
    return EXIT_SUCCESS;
}


/// Prompts for a size_t value once
/// \param value - A pointer to the size_t
/// \return EXIT_FAILURE if the supplied input doesn't match
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
    free(tokens);
    return EXIT_SUCCESS;
}


/// Prompts for a size_t until a valid value has been entered
/// \return The size_t value
size_t prompt_for_size_t() {
    size_t result;
    while (get_size_t(&result));
    return result;
}


/// Prompts for an integer until a valid value has been entered
/// \return The int value
int prompt_for_int() {
    int result;
    while (get_integer(&result));
    return result;
}


/// Prompts for at least number_of_tokens tokens until valid tokens have been entered
/// \param line - The buffer which holds the string
/// \param tokens - The buffer holding the tokens
void prompt_for_tokens(char *line, char**tokens, const size_t number_of_tokens) {
    do {
        get_line(line, BUFSIZ);
    } while (get_tokens(line, tokens, number_of_tokens));
}
