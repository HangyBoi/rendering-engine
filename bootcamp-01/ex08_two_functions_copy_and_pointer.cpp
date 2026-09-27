#include <cstdio>

// Task (Ex 8): two functions, one takes int by value and one by pointer;
// each multiplies by 2. Shows which one actually changes the caller's variable.
void multiplyByValue(int P) {
    P = P * 2;              // only changes the copy
}

void multiplyByPointer(int* pP) {
    *pP = *pP * 2;          // changes the original, through its address
}

int main() {
    int A = 10;

    multiplyByValue(A);
    printf("after multiplyByValue(A): A = %d\n", A);
    multiplyByPointer(&A);
    printf("after multiplyByPointer(A): A = %d\n", A);

    return 0;
}
