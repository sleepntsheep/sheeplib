#define SHEEP_FMT_IMPLEMENTATION
#include "../fmt.h"

typedef struct {
    const char *type;
    int age;
} cat_t;

void cat_json(FILE *fp, const void *const cat) {
    printf("{\n\ttype: %s,\n\tage: %d\n}",
            ((const cat_t *const)cat)->type,
            ((const cat_t *const)cat)->age);
}

int main() {
    fmt_register("cat", cat_json);
    fmt("Cat!: {cat}\n\n", &(cat_t) {
            .age = 5,
            .type = "Normal Cat!"
    });
    int a = 42;
    fmt("int: {int}\n", a);
    fmt("uint: {uint}\n", -1);
    fmt("long: {long}\n", 15394598458989L);
    fmt("longlong: {longlong}\n", 3409805394598458989LL);
    fmt("ulonglong: {ulonglong}\n", 11409805394598458989ULL);
    fmt("float: {float}\n", 1.0f/0.0f);
    fmt("longdouble: {longdouble}\n", 12349123471234781279834798127089573289450320745890237984705892739845082379457297305273457.0L);
    fmt("Pointer: {ptr}\n", &a);
    fmt("Wrongfmt: {int }\n");
    fmt("Wrongfmt2: {help}\n");
}

