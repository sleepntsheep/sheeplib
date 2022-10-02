#include<stdio.h>
#include <string.h>
const char *amogus = "ඞ";
const char *amogus3 = "\x0D\x9E";

int main() {
    printf("%d\n", strcmp(amogus, amogus2));
    for (int i = 0; i < strlen(amogus); i++) {
        printf("%hhd ", amogus[i]);
    }
    puts("");
    for (int i = 0; i < strlen(amogus2); i++) {
        printf("%hhd ", amogus2[i]);
    }
    

}
