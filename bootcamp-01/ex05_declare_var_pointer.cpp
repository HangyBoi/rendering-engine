#include <cstdio>

// Task (Ex 5): declare a regular int A and a pointer to it, pA; print both.
int main() {
    int A = 2;
    int* pA = &A;

    printf("A = %d\n", A);
    printf("Address of A = %p\n", (void*)pA);
    // Bug: label says "Address of pA" but this recomputes &A again (same value
    // as the line above). To show where pA itself lives, print (void*)&pA.
    printf("Address of pA = %p\n", (void*)&A);
    printf("*pA = %d\n", *pA);

    return 0;
}
