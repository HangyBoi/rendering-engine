#include <cstdio>

// Task (Ex 5-6): as ex05, then change A directly, then through the pointer,
// then repoint the pointer itself -- print after each step.
int main() {
    // Exercise 5
    int A = 2;
    int* pA = &A;

    printf("1) A = %d\n", A);
    printf("2) pA = %p (an address)\n", (void*)pA);
    printf("3) &A = %p (an address)\n", (void*)&A);
    printf("4) *pA = %d\n", *pA);
    printf("----------\n");

    // Exercise 6
    A = 4;              // change the variable
    printf("I. After 'A = 4': A = %d, *pA = %d, and address = %p\n", A, *pA, (void*)&A);
    *pA = 5;            // change the variable THROUGH the pointer
    printf("II. After '*pA = 5': A = %d, *pA = %d, and address = %p\n", A, *pA, (void*)&A);

    int other = 99;
    pA = &other;        // change the pointer itself: it now points elsewhere
    // Note: "address" below is always &A, which never changes -- it doesn't show
    // that pA now points elsewhere. (void*)pA would show that instead.
    printf("III. After 'pA = &other': A = %d, *pA = %d, and address = %p\n", A, *pA, (void*)&A);

    // No-op: pA already points at `other`, so this writes other's value back into itself.
    *pA = other;
    printf("IV. After '*pA = other': A = %d, *pA = %d, and address = %p\n", A, *pA, (void*)&A);

    return 0;
}
