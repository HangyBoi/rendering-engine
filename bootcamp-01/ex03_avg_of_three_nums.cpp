#include <cstdio>

// Task: declare three hardcoded ints, print their average.
int main() {
    int a = 5;
    int b = 10;
    int c = 20;

    float average = (a + b + c) / 3.0f; // 3.0f forces floating point division

    printf("The average between %d, %d and %d is %f\n", a, b, c, average);

    return 0;
}
