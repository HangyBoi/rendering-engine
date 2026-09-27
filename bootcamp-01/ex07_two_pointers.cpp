#include <cstdio>

// Task (Ex 5-7): as ex06, then add a second pointer pB to A and compare
// pA == pB. Below that: deliberate printf format/type mismatches, to see
// what happens when the format code doesn't match the argument.
int main() {
    // Exercise 5
    int A = 10;
    int* pA = &A;

    printf("1) A = %d\n", A);
    printf("2) pA = %p (an address)\n", (void*)pA);
    printf("3) &A = %p (an address)\n", (void*)&A);
    printf("4) *pA = %d\n", *pA);
    printf("----------\n");

    // Exercise 6
    A = 20;              // change the variable
    printf("I. After 'A = 20': A = %d, *pA = %d, and address = %p\n", A, *pA, (void*)&A);

    *pA = 30;            // change the variable THROUGH the pointer
    printf("II. After '*pA = 30': A = %d, *pA = %d, and address = %p\n", A, *pA, (void*)pA);

    int other = 99;
    pA = &other;        // change the pointer itself: it now points elsewhere
    printf("III. After 'pA = &other': A = %d, *pA = %d, and address = %p\n", A, *pA, (void*)pA);

    // No-op: pA already points at `other`, so this writes other's value back into itself.
    *pA = other;
    printf("IV. After '*pA = other': A = %d, *pA = %d, and address = %p\n", A, *pA, (void*)pA);

    // Exercise 7
    pA = &A;
    int* pB = &A;
    *pB = 55;
    printf("A = %d, *pA = %d, *pB = %d\n", A, *pA, *pB);
    printf("PA == PB ? %d\n", pA == pB);

    printf("---------\n");
    printf("Playing around and testing:\n");
    // %p wants a real pointer (void*). A and *pA below are plain ints -> undefined
    // behaviour: prints garbage (their value in hex, not a real address).
    // (void*)A is a legal but meaningless cast: turns the number 30 into a fake
    // "address" (0x1e). &A, pA, &pA (no cast) print real addresses and work on
    // every real compiler, but only the (void*)-cast versions are guaranteed by
    // the standard -- that's the version worth keeping as a habit.
    printf("%p\n" ,A);
    printf("%p\n" ,&A);
    printf("%p\n" ,(void*)A);
    printf("%p\n" ,(void*)&A);
    printf("%p\n" ,pA);
    printf("%p\n" ,&pA);
    printf("%p\n" ,*pA);
    printf("%p\n" ,(void*)pA);
    printf("%p\n" ,(void*)&pA);
    printf("---------\n");
    // %d wants a plain int. A and *pA are the only real ints here -> correct.
    // Every other line hands %d a pointer; on a 64-bit build that's 8 bytes read
    // as if it were a 4-byte int, so it prints a truncated, meaningless number
    // (not a crash -- just noise, since nothing here is actually dereferenced).
    printf("%d\n" ,A);
    printf("%d\n" ,&A);
    printf("%d\n" ,(void*)A);
    printf("%d\n" ,(void*)&A);
    printf("%d\n" ,pA);
    printf("%d\n" ,&pA);
    printf("%d\n" ,*pA);
    printf("%d\n" ,(void*)pA);
    printf("%d\n" ,(void*)&pA);

    return 0;
}
