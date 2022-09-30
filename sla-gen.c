#include <stdio.h>

FILE *f;

typedef struct {
    const char *prefix;
    const char *name;
} type_t;

#define TYPEDECL(prefix, name) static const type_t name = {prefix, name}

void genvec(int dimension, const char *prefix, const char *type) {
    if (dimension >= 5) {
        fprintf(stderr, "Not Implemented\n");
        return;
    }
    fprintf(f, "typedef struct { %s ", type);
    for (int i = 0; i < dimension - 1; i++)
        fprintf(f, "%c, ", 'x' + i);
    fprintf(f, "%c } Vec%d%s;\n", dimension - 1 + 'x', dimension, prefix);
}

void genheader() {
    fprintf(f, "#pragma once\n"
               "#ifndef SLA_H_\n"
               "#define SLA_H_\n");
}

void genfooter() { fprintf(f, "#endif SLA_H_\n"); }

int main() {
    f = fopen("sla.h", "w");
    genheader();
    for (int i = 1; i < 4; i++) {
        genvec(i, "f", "float");
        genvec(i, "i", "int");
        genvec(i, "u", "unsigned int");
        genvec(i, "d", "double");
    }
    genfooter();
    fclose(f);
}
