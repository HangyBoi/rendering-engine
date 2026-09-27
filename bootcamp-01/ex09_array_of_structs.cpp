#include <cstdio>
#include <string>

// Lecture example (not one of the 12 exercises): array of structs, a static
// array, a dynamically allocated array, and pointer arithmetic through it.
typedef unsigned short s_int;

struct saxion_Student {
    std::string name;
    s_int age;
    s_int number;
};

int main() {
    saxion_Student joe;
    joe.name = "Joe";
    joe.age = 18;
    joe.number = 444;

    // A statically allocated array of 16 students
    saxion_Student classA[16];
    classA[0].name = joe.name;
    classA[0].age = joe.age;
    classA[0].number = joe.number;

    classA[1].name = "John";
    classA[1].age = 28;
    classA[1].number = 111;

    printf("Student.%s.\n--Age: %d.\n--Number: %d.\n",
        classA[1].name.c_str(), classA[1].age, classA[1].number);

    size_t studentSize = sizeof(saxion_Student);
    size_t classA_count = sizeof(classA) / studentSize;
    printf("Size of struct: %zu.\nNumber of elements in classA: %zu.\n",
        studentSize, classA_count);

    // A dynamically allocated array of 2 students
    int allocateThisAmount = 2;
    saxion_Student* classB = new saxion_Student[allocateThisAmount];

    classB[0].name = "George";
    classB[0].age = 49;
    classB[0].number = 666;
    classB[1].name = "Ana";
    classB[1].age = 21;
    classB[1].number = 777;

    printf("sizeof(classB) = %zu (a pointer, not the array)\n", sizeof(classB));
    printf("classB      = %p\n",(void*)classB);
    printf("&classB[0] = %p\n", (void*)&classB[0]);
    //  The struct holds a 32-byte std::string and two 2-byte numbers, 36 bytes in total,
    //  and the compiler pads it to 40 so that the next element starts on a properly aligned address.
    printf("&classB[1] = %p\n", (void*)&classB[1]);

    saxion_Student* pointer = &classB[0];
    printf("Student.%s.\n--Age: %d.\n--Number: %d.\n--Address: %p\n",
       pointer->name.c_str(), pointer->age, pointer->number,(void*)&pointer[0]);

    pointer++;
    // Careful: pointer now IS classB[1], so &pointer[1] points one PAST it (the
    // nonexistent classB[2]) -- not this student's own address. &pointer[0] (or
    // just pointer) would print classB[1]'s real address.
    printf("Student.%s.\n--Age: %d.\n--Number: %d.\n--Address: %p\n",
       pointer->name.c_str(), pointer->age, pointer->number, (void*)&pointer[1]);

    delete[] classB;
    return 0;
}
