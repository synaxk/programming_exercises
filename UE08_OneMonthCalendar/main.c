#include <stdio.h>

int main() {
    int numOfDays = 0;
    int day = 1;
    int start = 1;
    printf("number of days in month: ");
    scanf("%d", &numOfDays);
    printf("starting day of week (1=Monday, 7=Sunday): ");
    scanf("%d", &start);
    printf("\n");

    for (int i = 0; day <= numOfDays; i++) {
        if (i >= start) {
            printf("%2d ", day++);
        } else {
            printf("   ");
        }
        if (i % 7 == 0) {
            putchar('\n');
        }
    }
    putchar('\n');
    return 0;
}