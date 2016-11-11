

typedef struct input {
    char *line;
    size_t amount_of_tokens;
    char **tokens;
} Input;

Input *create_input(size_t amount_of_tokens) {
    Input result = malloc(sizeof(Input));
    result.line = malloc(BUFSIZ);
    result.amount_of_tokens = amount_of_tokens;
    result.tokens = malloc(sizeof(char *) * amount_of_tokens);
    return result;
}

Input *destroy_input(Input *input) {
    if (input) {
        free(input->tokens);
        free(input->line);
        free(input);
    }
    return NULL;
}


char **get_tokens(Input *input) {

    // Get the line
    char line[BUFSIZ];
    fgets(line, sizeof(line), stdin);
    printf("%s", line);

    // Get the tokens
    char **tokens = malloc(amount_of_tokens * sizeof(char *));
    char *delimiter = " ";
    char *ptr = strtok(line, delimiter);

    for (size_t i = 0; i < amount_of_tokens; ++i) {
        printf("Token: %s\n", ptr);
        tokens[i] = malloc(strlen(ptr) * sizeof(char));
        tokens[i] = ptr;
        ptr = strtok(NULL, delimiter);
    }

    return tokens;
}
