#include <stdio.h>

int main() {
    int nums[5], min, max, sum;
    max = sum = 0;
    min = 9;
    printf(": ");

    scanf("%d %d %d %d %d", nums, nums + 1, nums + 2, nums + 3, nums + 4);

    for (int i = 0; i < 5; i++) {
        if (nums[i] > max)
            max = nums[i];
        if (nums[i] < min)
            min = nums[i];
        sum += nums[i];
    }

    printf("minimum sum: %d\nmaximum sum: %d", sum - max, sum - min);
    return 0;
}
