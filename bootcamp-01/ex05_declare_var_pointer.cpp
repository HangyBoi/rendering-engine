#include <cstdio>

// Task (Ex 5): declare a regular int A and a pointer to it, pA; print both.
int main() {
    int A = 2;
    int* pA = &A;

    printf("A = %d\n", A);
    printf("Address of A = %p\n", (void*)pA);
    printf("Address of pA = %p\n", (void*)&pA);
    printf("*pA = %d\n", *pA);

    return 0;
}
