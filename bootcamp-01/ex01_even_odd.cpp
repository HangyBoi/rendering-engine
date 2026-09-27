#include <cstdio>

// Task: declare one hardcoded int, print whether it's even or odd.
int main() {
    int number = 7;

    if (number % 2 == 0) {   // == 0 for even, everything else is odd (works for negatives too)
        printf("%d is even\n", number);
    } else {
        printf("%d is odd\n", number);
    }
    return 0;
}
