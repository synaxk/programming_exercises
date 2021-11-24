#include <stdio.h>
#include <stdlib.h>

enum node_type {
    operator_type,
    number_type
};

struct node {
    char operator;
    int number;
    enum node_type type;
    struct node *next;
};

struct node *createOperatorNode(char operator);
struct node *createNumberNode(int number);
struct node *findFirstPointOperator(struct node *head); // NULL, wenn nicht gefunden
struct node *findFirstDashOperator(struct node *head); // NULL, wenn nicht gefunden
struct node *findPreviousNode(struct node *head, struct node *node);
void removeAfterNode(struct node *node);
struct node *addLast(struct node *head, struct node *newNode);
void printList(struct node *head);
struct node *addFirst(struct node *newNode);
int getInput();
int isNumOrOp(int c);
void calculate(struct node* head);

int main() {
    int c;
    struct node *list = NULL;
    struct node *newNode;

    while ((c = getInput()) != '=') {
        if (isNumOrOp(c) == 1) {
            newNode = createNumberNode(c);
        } else if (isNumOrOp(c) == 2) {
            newNode = createOperatorNode((char) c);
        } else {
            printf("\n\n!!!!UPSALA!!!\n\n");
            return 1;
        }

        if (list == NULL) {
            list = addFirst(newNode);
            printf("\nCurrent list: ");
            printList(list);
            printf("\n");
        } else {
            addLast(list, newNode);
            printf("\nCurrent list: ");
            printList(list);
            printf("\n");
        }
    }

    calculate(list);

    return 0;
}

int getInput() {
    static int i = 0;
    int input = 0;
    if (i++ % 2 == 0) {
        printf("\nEnter number: ");
        scanf(" %d", &input);
    } else {
        printf("\nEnter operator: ");
        scanf(" %c", (char*)&input);
    }
    return input;
}

int isNumOrOp(int c) {
    if (c == '+' || c == '-' || c == '/' || c == '*') {
        return 2;
    } else {
        return 1;
    }
}

struct node *createNumberNode(int number) {
    struct node *numberNode = (struct node *) malloc(sizeof(struct node));
    numberNode->type = number_type;
    numberNode->number = number;
    numberNode->next = NULL;
    return numberNode;
}

struct node *createOperatorNode(char operator) {
    struct node *operatorNode = (struct node *) malloc(sizeof(struct node));
    operatorNode->type = operator_type;
    operatorNode->operator = operator;
    operatorNode->next = NULL;
    return operatorNode;
}

struct node *addFirst(struct node *newNode) {
    struct node *newHead = newNode;
    newHead->next = NULL;
    return newHead;
}

struct node *addLast(struct node *head, struct node *newNode) {
    if (head->next == NULL) {
        head->next = newNode;
        return newNode;
    } else {
        return addLast(head->next, newNode);
    }
}


void printList(struct node *head) {
    if (head->type) {
        printf("%d", head->number);
    } else {
        printf("%c", head->operator);
    }
    if (head->next != NULL) {
        printList(head->next);
    }
}

struct node *findFirstPointOperator(struct node *head) {
    if (head->type == operator_type && (head->operator == '*' || head->operator == '/')) {
        return head;
    } else if (head->next != NULL) {
        return findFirstPointOperator(head->next);
    } else {
        return NULL;
    }
}

struct node *findFirstDashOperator(struct node *head) {
    if (head->type == operator_type && (head->operator == '+' || head->operator == '-')) {
        return head;
    } else if (head->next != NULL) {
        return findFirstDashOperator(head->next);
    } else {
        return NULL;
    }
}

struct node *findPreviousNode(struct node *head, struct node *node) {
    if (head->next == node) {
        return head;
    } else if (head->next != NULL) {
        return findPreviousNode(head->next, node);
    } else {
        return NULL;
    }
}

void removeAfterNode(struct node *node) {
    struct node* tmp;
    if (node->next != NULL) {
        tmp = node->next;
        if (tmp->next != NULL) {
            node->next = tmp->next;
            free(tmp);
            tmp = NULL;
        } else {
            free(node->next);
            node->next = NULL;
        }

    }
}

void calculate(struct node* head) {
    struct node* tmp;
    struct node* prevNode;
    while ((tmp = findFirstPointOperator(head)) != NULL) {
        prevNode = findPreviousNode(head, tmp);
        if (tmp->operator == '*') {
            prevNode->number *= tmp->next->number;
        } else {
            if (tmp->next->number == 0) {
                tmp->next->number = 1;
            }
            prevNode->number /= tmp->next->number;
        }
        removeAfterNode(tmp);
        removeAfterNode(prevNode);
        printf("\nPoint operator result: ");
        printList(head);
        printf("\n");
    }

    while ((tmp = findFirstDashOperator(head)) != NULL) {
        prevNode = findPreviousNode(head, tmp);
        if(tmp->operator == '-') {
            prevNode->number -= tmp->next->number;
        } else {
            prevNode->number += tmp->next->number;
        }
        removeAfterNode(tmp);
        removeAfterNode(prevNode);
        printf("\nDash operator result: ");
        printList(head);
        printf("\n");
    }

    printf("\nFinal result: ");
    printList(head);
    free(head);
}