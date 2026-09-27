#include <cstdio>

// Task (Ex 9): static int[10], fill with 1..10 in a loop, print on one line.
int main() {
    int numbers[10];

    for (int i = 0; i < 10; i++) {
        numbers[i] = i+1;
        printf("%d ", numbers[i]);
    }
    printf("\n");
    return 0;
}
