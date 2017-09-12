#include <stdio.h>

int main() {
    char * data = "\x66\xec\xee\x39";
    float * foo = (float*)data;

    printf("%f\n", *foo);
}
