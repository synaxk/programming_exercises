#include <stdio.h>
#include <stdlib.h>

struct list {
    int number;
    struct list* next;
};

struct list* addElement(int number, struct list* head) {
    struct list* newHead;
    newHead = (struct list*)malloc(sizeof(struct list));
    newHead->number = number;
    newHead->next = head;
    return newHead;
}

struct list* removeElement(struct list* head) {
    if (head == NULL) {
        return head;
    }

    struct list* newHead = head->next;
    free(head);

    return newHead;
}

int main () {

    int input = 0;
    struct list* myList = NULL;
    scanf("%d", &input);
    while (input != 0) {
        myList = addElement(input, myList);
        scanf("%d", &input);
    }

    while (myList != NULL) {
        printf("%d ", myList->number);
        myList = removeElement(myList);
    }
}