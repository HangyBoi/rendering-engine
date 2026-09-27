#include <cstdio>

// Task: declare four hardcoded ints, print their sum.
int sum4(int a, int b, int c, int d) {
    int sum = a + b + c + d;
    return sum;
}

int main() {
    int a = 3;
    int b = 4;
    int c = 5;
    int d = 6;

    int summation = sum4(a, b, c, d);

    printf("The summation of four numbers is %d\n", summation);
    return 0;
}
