#include <cstdio>

// Task (Ex 11): function taking an array + its size, returns the smallest
// value; array is built, called, and printed from main.
int smallest(const int* values, int size) {
    int min = values[0];
    for (int i = 1; i < size; i++) {
        if (values[i] < min) {
            min = values[i];
        }
        printf("current min = %d\n", min); // trace line, not required output
    }
    printf("final min = %d\n", min); // trace line, not required output
    return min;
}

int main() {
    int values[10] = {15, 34, 7, 44, -11, 98, 65, 54, 22, 0};

    int size = sizeof(values) / sizeof(values[0]); // correct: values is still a real array here, not a pointer yet
    int result = smallest(values, size);

    printf("The smallest number is %d\n", result);
    return 0;
}
