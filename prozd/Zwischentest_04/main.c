#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct innode {
    char letter;
    struct innode *next;
} InnerNode;

typedef struct node {
    InnerNode *word;
    struct node *next;
} Node;


char getOption(char *opts, char *prompt);

InnerNode *addLetter(InnerNode *head, InnerNode *newLetter);

Node *getWord();

Node *addWord(Node *head, Node *newWord);

Node *insertWord(Node *head, Node *newWord, int index);

Node *deleteWord(Node *head, int index);

void printWords(Node *head);

void printLetters(InnerNode *head);

void freeLetters(InnerNode *head);

void freeWords(Node *head);

int compare_alphabetically(InnerNode *m, InnerNode *n);

int compare_length(InnerNode *m, InnerNode *n);

Node *sort(Node *head, int (*compare)(InnerNode *m, InnerNode *n), int descending);

int main() {
    char option = 0;
   // char *menu = "\nChoose action: (a)ppend, (i)nsert, (d)elete, (s)ort, (p)rint, e(x)it: ";
    const char menu[] = "\nChoose action: (a)ppend, (i)nsert, (d)elete, (s)ort, (p)rint, e(x)it: ";
    char *sortTypeOpts = "\nSort (a)lphabetically or by (l)ength? ";
    char *sortOrder = "\nSort (a)scending or (d)escending? ";

    int index = 0;
    Node *words = NULL;
    char sortType;
    int descending = 0;

    while ((option = getOption("adispx", menu)) != 'x') {
        switch (option) {
            case 'a':
                if (words == NULL) {
                    words = addWord(words, getWord());
                } else {
                    addWord(words, getWord());
                }

                break;
            case 'i':
                index = (int) getOption("0123456789", "\nEnter Index:");
                if (index == 0) {
                    words = insertWord(words, getWord(), index);
                } else {
                    insertWord(words, getWord(), index);
                }
                break;
            case 'd':
                index = (int) getOption("0123456789", "\nEnter Index:");
                if (index == 0) {
                    words = deleteWord(words, index);
                } else {
                    deleteWord(words, index);
                }
                break;
            case 's':
                sortType = getOption("al", sortTypeOpts);
                descending = ((int) getOption("ad", sortOrder) == 'a') ? 0 : 1;

                if (sortType == 'a') {
                    sort(words, &compare_alphabetically, descending);
                } else if (sortType == 'l') {
                    sort(words, &compare_length, descending);
                }
                break;
            case 'p':
                printf("\n");
                printWords(words);
                break;
        }
    }
    if (words != NULL) {
        freeWords(words);
        free(words->word);
        free(words);
    }

    return 0;
}

Node *getWord() {
    int c;
    Node *head = (Node *) malloc(sizeof(Node));
    head->word = NULL;
    InnerNode *newLetter = NULL;
    printf("\nEnter Word: ");
    while ((c = getchar()) != '\n');
    while ((c = getchar()) != '\n') {
        newLetter = (InnerNode *) malloc(sizeof(InnerNode));
        newLetter->letter = (char) c;
        newLetter->next = NULL;
        if (head->word == NULL) {
            head->word = addLetter(head->word, newLetter);
        } else {
            addLetter(head->word, newLetter);
        }
    }
    head->next = NULL;
    return head;
}

InnerNode *addLetter(InnerNode *head, InnerNode *newLetter) {
    if (head == NULL) {
        head = newLetter;
        return head;
    } else if (head->next == NULL) {
        return head->next = newLetter;
    } else {
        return addLetter(head->next, newLetter);
    }

}

Node *addWord(Node *head, Node *newWord) {
    if (head == NULL) {
        head = newWord;
        return head;
    } else if (head->next == NULL) {
        return head->next = newWord;
    } else {
        addWord(head->next, newWord);
        return head;
    }
}

Node *insertWord(Node *head, Node *newWord, int index) {
    for (int i = 1; i < index; i++) {
        head = head->next;
    }

    if (index == 0) {
        newWord->next = head;
        return newWord;
    }
    newWord->next = head->next;
    head->next = newWord;
    return newWord;
}

Node *deleteWord(Node *head, int index) {
    Node *tmp = NULL;
    for (int i = 1; i < index; i++) {
        head = head->next;
    }
    if (index == 0) {
        tmp = head->next;
        freeLetters(head->word);
        free(head->word);
        free(head);
        return tmp;
    }
    freeLetters(head->next->word);
    free(head->next->word);
    head->next->word = NULL;
    if (head->next->next != NULL) {
        tmp = head->next->next;
    }
    free(head->next);
    head->next = tmp;
    return head;
}

void freeLetters(InnerNode *head) {
    if (head->next == NULL) {
        return;
    }
    if (head->next->next != NULL) {
        freeLetters(head->next);
    }
    free(head->next);
    head->next = NULL;
}

void freeWords(Node *head) {
    if (head->next == NULL) {
        freeLetters(head->word);
        return;
    }
    if (head->next->next != NULL) {
        freeWords(head->next);
    }
    if (head->next->word != NULL) {
        freeLetters(head->next->word);
        free(head->next->word);
        head->next->word = NULL;
    }

    free(head->next);
    head->next = NULL;
    freeLetters(head->word);
    free(head->word);
    head->word = NULL;
}

void printWords(Node *head) {
    if (head == NULL) {
        printf("empty list");
        return;
    } else {
        printLetters(head->word);
        printf(" ");
    }
    if (head->next != NULL) {
        printWords(head->next);
    }
}

void printLetters(InnerNode *head) {
    if (head == NULL) {
        printf("\nempty word ");
    } else {
        printf("%c", head->letter);
    }

    if (head->next != NULL) {
        printLetters(head->next);
    }
}

char getOption(char *opts, char *prompt) {
    int optsLength = 0, valid = 0;
    char op = 0;
    printf("%s", prompt);
    scanf(" %c", &op);
    /*get string length of opts for the loop*/
    optsLength = strlen(opts);
    /*loop through chars (valid options) of opts*/
    for (int i = 0; i < optsLength; i++) {
        /*set valid to 1 if input matches one of the options*/
        if (op == *(opts + i)) {
            if (op >= '0' && op <= '9') {
                op -= '0';
            }
            valid = 1;
        }
    }
    if (valid) {
        return op;
    } else {
        op = getOption(opts, prompt);
    }
    return op;
}

int compare_alphabetically(InnerNode *m, InnerNode *n) {
    int valueM = m ? m->letter : 0; // ASCII value of the letter
    int valueN = n ? n->letter : 0; // or 0 when node is NULL
    int diff = valueM - valueN; // indicates which is the higher value
    if (diff == 0) { // on equal value
        if (m != NULL && n != NULL) { // if both lists have another entry
            // continue recursively
            return compare_alphabetically(m->next, n->next);
        }
    }
    return diff;
}

int compare_length(InnerNode *m, InnerNode *n) {
    int valueM = m ? 1 : 0;
    int valueN = n ? 1 : 0;
    int diff = valueM - valueN; // indicates which is the higher value
    if (diff == 0) { // on equal value
        if (m != NULL && n != NULL) { // if both lists have another entry
            // continue recursively
            return compare_length(m->next, n->next);
        }
    }
    return diff;
}

Node *sort(Node *head, int (*compare)(InnerNode *m , InnerNode *n), int descending) {
    int num = 1;
    Node *currentNode;
    InnerNode *tmpWord;

    currentNode = head;
    while (currentNode->next != NULL) {
        num++;
        currentNode = currentNode->next;
    }


    for (int i = 0; i < num - 1; i++) {
        currentNode = head;
        for (int j = 0; j < num - 1 - i; j++) {
            if (descending == 0) {
                if ((*compare)(currentNode->word, currentNode->next->word) > 0) {
                    tmpWord = currentNode->word;
                    currentNode->word = currentNode->next->word;
                    currentNode->next->word = tmpWord;
                }
            } else {
                if ((*compare)(currentNode->word, currentNode->next->word) < 0) {
                    tmpWord = currentNode->word;
                    currentNode->word = currentNode->next->word;
                    currentNode->next->word = tmpWord;
                }

            }
            currentNode = currentNode->next;
        }

    }
    return head;
}