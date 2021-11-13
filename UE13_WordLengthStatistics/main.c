#include <stdio.h>

#define STRLEN 80

int isChar(int c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        return 1;
    }
    return 0;
}

int main() {
    char input[STRLEN];
    int c = 0;
    for (int i = 0; (c = getchar()) != '.' && i < 80 ; i++) {
        if (isChar(c)){
            
        }
    }
    return 0;
}
