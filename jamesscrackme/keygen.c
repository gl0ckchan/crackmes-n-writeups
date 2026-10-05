#include <stdio.h>
#include <string.h>

#define MIN_NAME 3
#define MAX_NAME 14

int main() {
    printf("name: ");

    char name[MAX_NAME];
    if (!fgets(name, MAX_NAME, stdin)) return 69;
    size_t name_len = strlen(name);

    if (name_len <= MIN_NAME) return 69;
    if (name[name_len - 1] == '\n') name[--name_len] = '\0';

    unsigned int sum = 0;
    for (size_t i = 0; i < name_len; ++i) {
        sum += name[i];
        sum -= 15487;
    }

    printf("serial: SR8-%d\n", sum);
}
