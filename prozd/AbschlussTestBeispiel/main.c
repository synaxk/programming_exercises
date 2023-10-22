#include <stdio.h>
#include <stdlib.h>


struct node *addFirst(struct node *head, struct node *newNode);
void removeNext(struct node *previous);
struct node *createNewNode(const int value, const int limit);
int getInput(int *value, int*limit);
void addValue(struct node *node, int value);
void printList(struct node *head);

struct node {
    int value;
    int limit;
    struct node *next;
};

int main () {
    int value = 0, limit = 0, sum = 0, nodeCount = 0, endCount = 0;

    struct node *list = NULL;
    struct node *newNode = NULL;

    while (getInput(&value, &limit)) {
        if (value > limit) {
            printf("\n\n");
            continue;
        }
        newNode = createNewNode(value, limit);
        nodeCount++;
        list = addFirst(list, newNode);
        addValue(list, value);

        printf("\n");
        printList(list);
        printf("\n");
    }

    struct node *tmp = list;

    while (tmp != NULL) {
        sum += tmp->value;
        endCount++;
        tmp = tmp->next;
    }
    printf("\n");
    printList(list);
    printf("\n");
    printf("Deleted: %d\n", nodeCount - endCount);
    printf("AVG    : %.2f", (float)sum/endCount);
    return 0;
}


struct node *addFirst(struct node *head, struct node *newNode) {
    newNode->next = head;
    return newNode;
}

struct node *createNewNode(const int value, const int limit) {
    struct node *newNode = malloc(sizeof(struct node));
    newNode->value = value;
    newNode->limit = limit;
    newNode->next = NULL;
    return newNode;
}

void removeNext(struct node* previous) {
    struct node* tmp = previous->next;
    previous->next = tmp->next;
    free(tmp);
    tmp = NULL;
}

int getInput(int *value, int *limit){
    printf("Enter value and limit X/Y: ");
    scanf("%d/%d", value, limit);
    if (*value == 0 || *limit == 0) {
        return 0;
    }
    return 1;
}

void addValue(struct node *prevNode, int value) {
    if (prevNode == NULL || prevNode->next == NULL) {
        return;
    }

    prevNode->next->value += value;

    if(prevNode->next->value > prevNode->next->limit) {
        removeNext(prevNode);
        addValue(prevNode, value);
    }
    addValue(prevNode->next, value);
}

void printList(struct node *head) {
    printf("%d/%d ", head->value, head->limit);
    if (head->next != NULL) {
        printList(head->next);
    }
}
