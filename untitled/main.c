#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Tipp: Initialisierung von String mit '\0' !!!!
//Tipp: in priorityType Typ 'empty' hinzufügen

enum priorityType {
    highest = 5,
    high = 4,
    normal = 3,
    low = 2,
    lowest = 1
};

struct entry {
    enum priorityType priority;
    char message[32];
};

struct pqueue {
    struct entry entries[10];
    int counter;
};

char getMenu() {
    char action;
    printf("\nChoose action: print queue (p), enqueue entry (e), dequeue entry (d) or exit (x): ");
    scanf(" %c", &action);
    while(action != 'p' && action != 'e' && action != 'd' && action != 'x') {
        printf("\nInput invalid! Try again: ");
        scanf(" %c", &action);
        if(action == 'p' || action == 'e' || action == 'd' || action == 'x') {
            return action;
        }
    }
    return action;
}


struct entry getNewEntry() {
    struct entry newEntry;
    char priority;
    char message[32];

    printf("\nChoose priority: lowest (L), low (l), normal (n), high (h), highest (H): ");
    scanf(" %c", &priority);
    while(priority != 'L' && priority != 'l' && priority != 'n' && priority != 'h' && priority != 'H') {
        printf("\nInput invalid! Try again: ");
        scanf(" %c", &priority);
    }
    switch(priority) {
        case 'H':
            newEntry.priority = highest;
            break;
        case 'h':
            newEntry.priority = high;
            break;
        case 'n':
            newEntry.priority = normal;
            break;
        case 'l':
            newEntry.priority = low;
            break;
        case 'L':
            newEntry.priority = lowest;
            break;
    }
    printf("\nChoose message: ");
    scanf("%s", message);
    strcpy(newEntry.message, message);
    return newEntry;

}




int isEmpty(struct pqueue* pqueue) {
    if(pqueue->counter == 0) {
        return 1;
    } else {
        return 0;
    }
}


int isFull(struct pqueue* pqueue) {
    if(pqueue->counter == 10) {
        return 1;
    } else {
        return 0;
    }
}


void enqueue(struct pqueue* pqueue, struct entry entry) {
    int correctPosition = 0;

    if(isEmpty(pqueue)) {
        pqueue->entries[0] = entry;
        pqueue->counter++;
        printf("\npriority queue now contains 1 entry");

    } else if(isFull(pqueue)) {
        printf("\npriority queue already full!");
    } else {
        for(int counterEnqueue = pqueue->counter; correctPosition == 0; counterEnqueue--) {
            if(entry.priority == pqueue->entries[counterEnqueue].priority) {
                correctPosition = counterEnqueue;
            }
        }
        for(int counterEnqueue = 9; counterEnqueue >= correctPosition; counterEnqueue--) {
            pqueue->entries[counterEnqueue] = pqueue->entries[counterEnqueue - 1];
        }
        pqueue->entries[correctPosition] = entry;
        pqueue->counter++;
        printf("\npriority queue now contains %d entries", pqueue->counter);
    }
}

void printQueue(struct pqueue* pqueue) {
    for(int counterPrint = 0; counterPrint < pqueue->counter; counterPrint++) {
        switch(pqueue->entries[counterPrint].priority) {
            case highest:
                printf("\nH: %s", pqueue->entries[counterPrint].message);
                break;
            case high:
                printf("\nh: %s", pqueue->entries[counterPrint].message);
                break;
            case normal:
                printf("\nn: %s", pqueue->entries[counterPrint].message);
                break;
            case low:
                printf("\nl: %s", pqueue->entries[counterPrint].message);
                break;
            case lowest:
                printf("\nL: %s", pqueue->entries[counterPrint].message);
                break;
            default: break;
        }

    }
}


int main() {

    char action;
    struct pqueue newQueue;
    newQueue.counter = 0;
    struct entry newEntry;

    while(action != 'x') {
        action = getMenu();
        switch(action) {
            //print queue
            case 'p':
                printQueue(&newQueue);
                break;
                //enqueue entry
            case 'e':
                newEntry = getNewEntry();
                enqueue(&newQueue, newEntry);
                break;
                //dequeue entry
            case 'd':
                return 0;
                break;
                //exit
            case 'x':
                return 0;
                break;
        }
    }

    //printf("%c", action);

    return 0;
}