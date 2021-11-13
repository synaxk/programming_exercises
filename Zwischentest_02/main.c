#include <stdio.h>
#include <string.h>

/* Zwischentest_02
 * Alexander Kast
 *
 * */

/*definitions*/
//priority enumeration
enum priority {
    highest = 5, high = 4, normal = 3, low = 2, lowest = 1
};
//struct for queue entries
struct entry {
    enum priority priority;
    char message[32];
};
//struct for priorityQueue
struct pQueue {
    struct entry entries[10];
    int counter;
};

//function prototypes
int isEmpty(struct pQueue *pQueue); // return 1 if queue is empty, 0 if not empty
int isFull(struct pQueue *pQueue); // return 1 if queue is full, 0 if not
void enqueue(struct pQueue *pQueue, struct entry entry); // insert entry into priorityQueue
void dequeue(struct pQueue *pQueue); // remove first entry from priorityQueue
void printQueue(struct pQueue *pQueue);
struct entry getNewEntry();
void moveDown(struct pQueue *pQueue, int index); //rearrange positions of priorityQueue
void moveUp(struct pQueue *pQueue, int index); //rearrange positions of priorityQueue
char getOption(char *opts); //get valid option
char getMenu(); // print menu options and get input
char getPriority(); //print priority options and get input
enum priority mapPriority(char input); // map char input to int values of enumeration
char mapBackPriority(enum priority priority); // map enum to char
void getMessage(char *message); // read message
int getPriorityIndex(struct pQueue *pQueue, struct entry *entry); // get next index to store the entry

int main() {
    /*declarations*/
    char option = '\0';
    struct entry newEntry;
    struct pQueue pQueue;     // step 1: declare a priority queue
    pQueue.counter = 0;

    // step 2: getMenu -> repeat until exit
    while ((option = getMenu()) != 'x') {
        if (option == 'p') {
            printQueue(&pQueue);
        } else if (option == 'e') {
            newEntry = getNewEntry();
            enqueue(&pQueue, newEntry);
        } else if (option == 'd') {
            dequeue(&pQueue);
        }
    }
    // exit
    return 0;
}

/* get valid input option, call recursively if not valid
 *  -   *opts are used to define valid options */
char getOption(char *opts) {
    int optsLength = 0, valid = 0;
    char op = 0;
    scanf(" %c", &op);
    /*get string length of opts for the loop*/
    optsLength = strlen(opts);
    /*loop through chars (valid options) of opts*/
    for (int i = 0; i < optsLength; i++) {
        /*set valid to 1 if input matches one of the options*/
        if (op == *(opts + i)) {
            valid = 1;
        }
    }
    /*return the option if it is valid, else print "Input invalid! Try again: ", and call the function recursively */
    if (valid) {
        return op;
    } else {
        printf("\nInput invalid! Try again: ");
        op = getOption(opts);
    }
    return op;
}

/* print menu options and get input option
 * "Choose action: print queue(p), enqueue entry(e), dequeue entry(d) or exit(x):
 *  -   if option is invalid, print "Input invalid! Try again:" and continue*/
char getMenu() {
    char option = '\0';
    printf("\nChoose action: print queue (p), enqueue entry (e), dequeue entry (d) or exit (x): ");
    option = getOption("pedx");
    return option;
}

char getPriority() {
    char option = '\0';
    printf("\nChoose priority: lowest (L), low (l), normal (n), high (h), highest (H): ");
    option = getOption("LlnhH");
    return option;
}

enum priority mapPriority(char input) {
    enum priority priority = normal;
    switch (input) {
        case 'L':
            priority = lowest;
            break;
        case 'l':
            priority = low;
            break;
        case 'n':
            priority = normal;
            break;
        case 'h':
            priority = high;
            break;
        case 'H':
            priority = highest;
            break;
        default:
            break;
    }
    return priority;
}

char mapBackPriority(enum priority priority) {
    char prio;
    switch (priority) {
        case lowest:
            prio = 'L';
            break;
        case low:
            prio = 'l';
            break;
        case normal:
            prio = 'n';
            break;
        case high:
            prio = 'h';
            break;
        case highest:
            prio = 'H';
            break;
        default:
            break;
    }
    return prio;
}

/*void getMessage(char *message) {
    printf("\nChoose message: ");
    scanf("%s", message);
    fflush(stdin);
}*/
void getMessage(char *message) {
    //todo: maybe add return value to handle invalid or too long messages
    printf("\nChoose message: ");
    int c = 0, i = 0;
    c = getchar();
    for (i = 0; (c = getchar()) != '\n' && i < 32; i++) {
        message[i] = c;
    }
    message[i] = '\0'; //increment i to last position of string and assign null terminator
}

/* getNewEntry;
 * get Priority and message -> validate and return entry */
struct entry getNewEntry() {
    struct entry newEntry;
    enum priority priority = mapPriority(getPriority());
    char message[32];
    getMessage(message);
    newEntry.priority = priority;
    strcpy(newEntry.message, message);
    return newEntry;
}

/* enqueue inserts entry into priorityQueue at the last position of its priority
 *  -   rearrange the position of later entries ( +1 )
 *  -   don't insert if the queue is already full
 *  -   onSuccess - print number of elements in queue "priority queue now contains %d entries"*/
void enqueue(struct pQueue *pQueue, struct entry entry) {
    int index = 0;
    if (isFull(pQueue)) {
        printf("\npriority queue already full!");
        return;
    }
    if (pQueue->counter != 0) {
        index = getPriorityIndex(pQueue, &entry);
        moveDown(pQueue, index); //rearrange positions after index
    }
    // write entry to priorityQueue at index position
    pQueue->entries[index] = entry;
    pQueue->counter++;
    if (pQueue->counter == 1) {
        printf("\npriority queue now contains %d entry", pQueue->counter);
    } else {
        printf("\npriority queue now contains %d entries", pQueue->counter);
    }

}


/* dequeue removes the first element of the queue and prints its message
 *  -   rearrange the positions of the other elements
 *  -   onSuccess - print number of elements in queue "priority queue now contains %d entries"
 *
 *  */
void dequeue(struct pQueue *pQueue) {
    if (!isEmpty(pQueue)) {
        printf("\nMessage: %s", pQueue->entries[0].message);
        moveUp(pQueue, 0);
        //decrement pQueue entry counter
        pQueue->counter--;
        if (pQueue->counter == 1) {
            printf("\npriority queue now contains %d entry", pQueue->counter);
        } else {
            printf("\npriority queue now contains %d entries", pQueue->counter);
        }
    } else {
        printf("\npriority queue is empty!");
    }
}

/* print the priority Queue in the following format from highest to lowest priority:
 * <priority>: <message>*/
void printQueue(struct pQueue *pQueue) {
    if (!isEmpty(pQueue)) {
        for (int i = 0; i < pQueue->counter; i++) {
            printf("\n%c: %s", mapBackPriority(pQueue->entries[i].priority), pQueue->entries[i].message);
        }
    } else {
        printf("\nempty queue");
    }
}

int getPriorityIndex(struct pQueue *pQueue, struct entry *entry) {
    enum priority currentPriority = entry->priority;
    int i = 0;
    for (; i < pQueue->counter; i++) {
        if (pQueue->entries[i].priority >= currentPriority) {
            continue;
        }
        return i;
    }
    return i;
}

/* rearrange the positions of the queue entries*/
void moveDown(struct pQueue *pQueue, int index) {
    for (int i = 9; i > index; i--) {
        pQueue->entries[i] = pQueue->entries[i - 1];
    }
}

/* rearrange the positions of the queue entries*/
void moveUp(struct pQueue *pQueue, int index) {
    for (int i = index; i < 9; i++) {
        pQueue->entries[i] = pQueue->entries[i + 1];
    }
}

int isEmpty(struct pQueue *pQueue) {
    if (pQueue->counter == 0) {
        return 1;
    } else {
        return 0;
    }
}

int isFull(struct pQueue *pQueue) {
    if (pQueue->counter == 10) {
        return 1;
    } else {
        return 0;
    }
}
