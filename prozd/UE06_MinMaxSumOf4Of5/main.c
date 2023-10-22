#include <stdio.h>

int main() {
    int nums[5], min, max, sum;
    max = sum = 0;
    min = 9;
    printf(": ");

    for (int i = 0; i < 5; i++) {
    	scanf("%d", nums + i);
        if (nums[i] > max)
            max = nums[i];
        if (nums[i] < min)
            min = nums[i];
        sum += nums[i];
    }

    printf("minimum sum: %d\nmaximum sum: %d", sum - max, sum - min);
    return 0;
}
