#include <cstdio>

// Task (Ex 10): dynamic int[10], fill with the first 10 odd numbers, print
// each on its own line.
int main() {
    int size = 10;

    int* odds = new int[size];

    for (int i = 0; i < size; i++) {
        odds[i] = 2 * i + 1;
        printf("%d \n", odds[i]);
    }

    delete[] odds;
    return 0;
}
