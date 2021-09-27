#include <stdio.h>
int power(int base, int power);

int main () {
    int oktNum, dezNum, digSum;
    oktNum = dezNum = digSum = 0;
    printf(": ");
    scanf("%3d", &oktNum);
    
    /*get the rightmost digit, multiply it with (8 to the power of i(=current position)),*/
    /*add to dezNum*/
    for (int i = 0; oktNum > 0; i++) {
        dezNum += (oktNum % 10) * power(8, i);
        /*truncate the rightmost digit*/
        oktNum /= 10;
    }
    
    printf("dec.: %d\n", dezNum);
    
    /*get the rightmost digit, add it to digSum*/
    for (;dezNum > 0;) {
        digSum += dezNum % 10;
        /*truncate the rightmost digit*/
        dezNum /= 10;
    }
    printf("ZS: %d\n", digSum);
    return 0;
}

/*multiply 'result' with 'base', repeat 'power'-times*/
int power(int base, int power) {
    int result = 1;
    for (int i = 0; i<power; i++) {
        result *= base;
    }
    return result;
}
