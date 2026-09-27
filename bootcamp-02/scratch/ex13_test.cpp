#include <cstdio>

    struct S {
        int l;
        int* ar;

        S(int l) {
            this->l=l;
            ar = new int[l];
            printf("%d %d\n", l, l);
        }

        void put(int index, int value) {
            ar[index] = value;
            printf("%d %d\n", index, ar[index]);
        }

        ~S() {
            delete[] ar;
        }
    };

int main() {
    S s1(5);
    S s2(10);

    printf("%d %d\n", s1.l, s2.l);

    s1.put(0,100);
    printf("%d\n", s1.ar[0]);
    s2.put(0,200);
    printf("%d\n", s2.ar[0]);

    {
        S s3(20);
    }

    return 0;
}