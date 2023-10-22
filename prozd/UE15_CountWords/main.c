#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEXTLENGTH 1000

char** scanWords();
int countWord(char *text, char *word);
void freeAll(char **strings);
void getInput(char *input);

int main() {
    char text[TEXTLENGTH+1];
    char **words;
    printf("text: ");
    getInput(text);

    printf("words:\n");
    words = scanWords();

    for (int i = 0; words[i][0] != '\0' ;i++) {
        countWord(text, words[i]);
    }
    freeAll(words);
    return 0;
}

char** scanWords() {
    char** words= ( char**)calloc(1, sizeof(char*)), **tmp;
    int i=0;
    while(1) {
        char buf[81];
        scanf("%80s", buf);
        if(buf[0]=='-') break;
        if(NULL==(tmp = ( char**)realloc(words, sizeof(char*)*(i+2)))) {
            printf("out of memory");
            break;
        }
        words=tmp;
        words[i] = ( char*)calloc(strlen(buf)+1, sizeof(char));
        strcpy(words[i], buf);
        i++;
    }
    words[i]=( char*)calloc(1,sizeof(char));
    return words;
}

void getInput(char *input) {
    int c = 0, i = 0;

    for (i = 0; (c = getchar()) != EOF && c != '\n'; i++) {
        input[i] = c;
    }
    input[++i] = '\0';
}

int countWord(char *text, char *word){
    int textLength = strlen(text);
    int wordLength = strlen(word);
    int counter = 0;

    for (int i = 0; i <= textLength; i++) {
        for (int j = 0; text[i] == word[j] && j < wordLength; j++, i++) {
            if (j == wordLength-1) {
                counter++;
                break;
            }
        }
    }
    printf("%s: %d\n", word, counter);
    return counter;
}

void freeAll(char **strings) {
    int i;

    for (i = 0; strings[i][0] != '\0'; i++) {
        free(strings[i]);
    }
    free(strings[i]);
    free(strings);
}