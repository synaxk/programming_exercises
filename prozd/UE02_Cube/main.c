#include <stdio.h>
/*UE02: Cube*/

int main() {
    int length = 0;
    printf(": ");
    scanf("%d", &length);
    printf("Surface area: %d\nVolume: %d\n", length*length*6, length*length*length);
    return 0;
}
