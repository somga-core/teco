#include <stdio.h>

int main() {
    unsigned char a = 0b101001;

    printf("%d, %d, %d, %d", a, a >> 2, (a >> 2) & 0b11, a >> 8);

    return 0;
}
