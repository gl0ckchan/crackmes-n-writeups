#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX_NAME 64

int main() {
    printf("name: ");

    char name[64];
    if (!fgets(name, 64, stdin)) return 69;
    size_t name_len = strlen(name);
    if (name[name_len - 1] == '\n') name[--name_len] = '\0';

    int sum = 0;
    for (size_t i = 0; i < name_len; ++i) {
        int char_converted = toupper(name[i]);
         sum = sum + char_converted;
    }

    int serial = sum ^ 0x5678 ^ 0x1234;

    printf("serial: %d\n", serial);

    return 0;
}
