#include <stdio.h>
#include <assert.h>
#define SHEEP_BIGINT_IMPLEMENTATION
#include "../sbi.h"

int main() {
    const char bs[] = "111111111111111111111111111111111111111";
    struct sbi *bi = sbi_from_cstr(bs);
    struct sbi *bi2 = sbi_from_cstr("222222222222222222222");
    assert(strcmp(sbi_to_cstr(bi), bs) == 0);

    struct sbi *res = sbi_add(bi, bi2);
    sbi_negate(res);

    puts(sbi_to_cstr(res));

    sbi_free(bi);
    return 0;
}


