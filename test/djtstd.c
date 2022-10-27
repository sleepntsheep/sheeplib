#include "../djtstd.h"
#define DJTSTD_IMPLEMENTATION
#include <stdio.h>


int main() {
	char a[20] = {0};
	memcpy(a, "TEST", 5);
	puts(a);

	return 0;
}
