#include <cstdio>

// Task: declare two hardcoded ints, print the highest of the two.
int main() {

    int a = 10;
    int b = 5;

    // My Solution
    // if (a > b) {
    //     printf("%d is greater than %d", a,b);
    // } else {
    //     printf("%d is not greater than %d", a, b);
    // }

    // Advanced solution: assume a is highest, let b challenge it
    // (same "keep best so far" pattern reused in Ex 11 with an array)
    int highest = a;
    if (b > highest) {
        highest = b;
    }

    printf("The highest number is %d\n", highest);
    return 0;
}
