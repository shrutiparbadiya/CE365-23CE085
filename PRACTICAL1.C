#include <stdio.h>
#include <string.h>

int main() {
    char str[100];
    int i = 0;

    printf("Enter the string: ");
    fgets(str, sizeof(str), stdin);

    while (str[i] == 'a') {
        i++;
    }

    if (str[i] == 'b' && str[i + 1] == 'b') {
        printf("Valid String\n");
    } else {
        printf("Invalid String\n");
    }

    return 0;
}
