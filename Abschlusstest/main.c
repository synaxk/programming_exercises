#include <stdio.h>
#include <stdlib.h>

struct node {
    int value;
    struct node *next;
};

int getInput(char *prompt);
struct node* createNewNode(const int value);
void printList(struct node* head);
struct node* addFirst(struct node *head, struct node *newNode);
struct node *insertNode(struct node *head, struct node *newNode);
struct node *removeFirst(struct node *head);
void filter(struct node *head, int min, int* deleted, int* avg);

int main() {
    int value = 0, min = 0, avg = 0, newAvg = 0, nodes = 0, deleted = 0;

    struct node *list = NULL;
    struct node *newNode = NULL;

    while ((value = getInput("\nEnter number: ")) != 0) {
        newNode = createNewNode(value);
        nodes++;
        avg += value;
        if (list == NULL) {
            list = addFirst(list, newNode);
        } else if (value < list->value) {
            list = insertNode(list, newNode);
        }else {
            insertNode(list, newNode);
        }
        printList(list);
    }

    min = getInput("\nEnter min: ");
    newAvg = avg;
    filter(list, min, &deleted, &newAvg);

    printf("\nDeleted Nodes: %d", deleted);
    printf("\nOld average: %d", avg/nodes);
    printf("\nNew average: %d", newAvg);

    return 0;
}

int getInput(char *prompt){
    int value = 0;
    printf("\n%s", prompt);
    scanf("%d", &value);
    return value;
}

struct node *createNewNode(const int value) {
    struct node *newNode = malloc(sizeof(struct node));
    newNode->value = value;
    newNode->next = NULL;
    return newNode;
}


void printList(struct node *head) {
    printf("%d", head->value);
    if (head->next != NULL) {
        printf("->");
        printList(head->next);
    } else {
        printf("->NULL");
    }
}

struct node *addFirst(struct node *head, struct node *newNode) {
    newNode->next = head;
    return newNode;
}

struct node *insertNode(struct node *head, struct node *newNode) {
    struct node *tmp = NULL;
    if (head->value < newNode->value && head->next != NULL) {
        return insertNode(head->next, newNode);
    } else if (newNode->value < head->value) {
        newNode->next = head;
        return newNode;
    } else {
        tmp = head->next;
        head->next = newNode;
        newNode->next = tmp;
        return head;
    }
}

struct node *removeFirst(struct node *head) {
    struct node *tmp = NULL;

    tmp = head->next;
    free(head);
    head = NULL;
    return tmp;
}

void filter(struct node *head, int min, int* deleted, int* avg) {
    while (head->value <= min) {
        deleted++;
        *avg -= head->value;
        head = removeFirst(head);
    }
}