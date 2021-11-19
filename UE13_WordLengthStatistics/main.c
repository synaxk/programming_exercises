#include <stdio.h>
#include <string.h>

#define LENGTHS 80


int isChar(int c);

int isValid(char *input);

void getInput(char *input);

int main() {
    char input[81];
    int lens[80];
    int length = 0;

    for (int i = 0; i < 80; i++) {
        lens[i] = 0;
    }

    getInput(input);

    if (!isValid(input)) {
        printf("NOT VALID\n");
        return 1;
    }

    int strLen = (int)strlen(input);

    for (int i = 0; i < strLen; i++) {
        if (isChar(input[i])) {
            length++;
        } else if (!isChar(input[i]) && length > 0) {
            lens[length]++;
            length = 0;
        } else {
            length = 0;
        }
    }

    for (int i = 1; i < strLen; i++) {
        if (lens[i] != 0) {
            printf("Length %d: %d\n", i, lens[i]);
        }
    }


    return 0;
}

void getInput(char *input) {
    int c = 0, i = 0;

    printf(": ");

    for (i = 0; (c = getchar()) != EOF && c != '\n' && i < 80; i++) {
        input[i] = c;
        if (input[i] == '.') {
            break;
        }
    }
    input[++i] = '\0';
}

int isChar(int c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        return 1;
    }
    return 0;
}

int isValid(char *input) {
    int len = (int) strlen(input);
    if (len >= 80) {
        return 0;
    }

    for (int i = 0; i < len; i++) {
        if (!isChar(input[i]) && input[i] != ' ' && input[i] != '.') {
            return 0;
        } else if (input[i] == '.' && i < len - 1) {
            return 0;
        } else if (input[i] == '.') {
            return 1;
        }
    }
    return 0;
}
