#include <stdio.h>

int main() {
    int lead = 1, pos = 1, num1 = 1;

    for (int i = 2; scanf("%d", &pos), pos >= 0;i++) {
        printf(": ");

        if (pos == 0) {
            --i;
            printf("final position #1: %d\n", num1);
            printf("winner: #%d\n", lead);
            return 0;
        }

        if (pos == 1) {
            num1++;
            printf("position #1: %d\n", num1);
            lead = i;
            printf("new leader: #%d\n", lead);
        } else if (pos <= num1) {
            num1++;
            printf("position #1: %d\n", num1);
        }
    }
}

