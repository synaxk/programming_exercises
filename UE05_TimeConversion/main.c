#include <stdio.h>

int main() {
    int hours, minutes, c;
    hours = minutes = 0;
    c = 'A';
    printf("24h time: ");
    scanf("%d:%d", &hours, &minutes);

    /*if hours > 12 subtract 12 and set period PM*/
    if (hours >= 12) {
        if (hours > 12)
            hours -= 12;
        c = 'P';
    } else if (hours == 0)
        hours = 12;
    printf("12h time: %2d:%02d %cM", hours, minutes, c);
    return 0;
}