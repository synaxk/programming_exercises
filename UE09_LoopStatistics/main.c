#include <stdio.h>

int main() {
    double num, min, max, avg;
    int i;
    num = max = avg = i = 0;
    min = 99;
    while (scanf("%lf", &num)) {
        printf(": ");
        if (num <= 0) {
            break;
        }
        if (num > max) {
            max = num;
        }
        if (num < min) {
            min = num;
        }
        i++;
        avg += num;
    }

    if (i > 0) {
        avg /= i;
        printf("cnt: %d\nmin: %.2f\nmax: %.2f\navg: %.2f\n",i, min, max, avg);
    } else {
        printf("no number entered\n");
    }
}