#include <stdio.h>

/*UE03: Tax*/

int main() {
    float num;
    printf("Enter an amount: ");
    scanf("%f", &num);
    printf("With tax added: %.2f", num/5*6);
    return 0;
}
