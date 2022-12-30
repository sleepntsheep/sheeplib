// usr/bin/clang "$0" && exec ./a.out "$@"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SHEEP_DYNARRAY_IMPLEMENTATION
#include "../dynarray.h"
#define SHEEP_STR_IMPLEMENTATION
#include "../str.h"
#define SHEEP_LOG_IMPLEMENTATION
#include "../log.h"
#define SNOW_ENABLED
#include "snow.h"

uint64_t intadd(uint64_t a, uint64_t b) { return a + b; }

int *rndarr(int size) {
    int i;
    int *a = arrnew;
    for (i = 0; i < arrlen(a); i++)
        arrpop(a);
    for (i = 0; i < size; i++) {
        int b = rand() % 65531;
        arrpush(a, b);
    }
    return a;
}

int cmpm(int a, int b) { return a - b; }

describe(dynarray) {
    it("array len") {
        int *a = arrnew;
        arrpush(a, 5);
        arrpush(a, 5);
        asserteq_int(arrlen(a), 2);
    }

    it("arrcap_test") {
        int *a = arrnew;
        asserteq_int(arrcap(a), 4);
        arrpush(a, 5);
        arrpush(a, 5);
        arrpush(a, 5);
        asserteq_int(arrcap(a), 4);
        arrpush(a, 5);
        asserteq_int(arrcap(a), 4);
        arrpush(a, 5);
        asserteq_int(arrcap(a), 8);
    }

    it("array push") {
        int *a = arrnew, i;
        for (i = 0; i < 10; i++) {
            arrpush(a, i);
            assert(a[i] == i);
        }
    }

    it("array del") {
        int *a = arrnew, i;
        for (i = 0; i < 5; i++)
            arrpush(a, i);
        arrdel(a, 2);
        asserteq_int(a[2], 3);
        asserteq_int(arrlen(a), 4);
        arrdel(a, 0);
        asserteq_int(a[0], 1);
    }

    it("array ins") {
        int *a = arrnew;
        int i, n;
        for (i = 0; i < 5; i++)
            if (i != 2)
                arrpush(a, i);
        asserteq_int(a[2], 3);
        asserteq_int(arrlen(a), 4);
        arrins(a, 2, 2);
        asserteq_int(a[2], 2);
        asserteq_int(a[3], 3);
        arrfree(a);
        a = arrnew;
        for (i = 0; i < 5; i++)
            arrpush(a, i);
        asserteq_int(a[3], 3);
        arrins(a, 3, -1);
        assert(a[3] == -1);
        arrins(a, 6, -2);
        assert(a[6] == -2);
        n = arrlen(a);
        for (i = n; i < n + 5; i++) {
            arrins(a, i, i);
            assert(a[i] == i);
        }
        arrfree(a);
    }

    it("arrsetlen_test") {
        int *a = arrnew, i;
        for (i = 0; i < 1000; i++)
            arrpush(a, i);
        asserteq_int(arrlen(a), 1000);
        arrsetlen(a, 500);
        asserteq_int(arrlen(a), 500);
        arrpush(a, -1);
        asserteq_int(arrlen(a), 501);
        assert(a[500] == -1);
        arrfree(a);
    }

    it("arrpop_test") {
        struct sa {
            int a, b;
        };
        struct sa ee, temp, *sa;
        int *a = arrnew;
        assert(sizeof(sa) == sizeof(int) * 2);
        arrpush(a, 1);
        arrpush(a, 1);
        arrpush(a, 1);
        arrpush(a, 2);
        arrpush(a, -1);
        asserteq_int(arrpop(a), -1);
        asserteq_int(arrpop(a), 2);
        asserteq_int(arrpop(a), 1);
        asserteq_int(arrpop(a), 1);
        asserteq_int(arrpop(a), 1);
        sa = arrnew;
        temp.a = 1;
        temp.b = 5;
        ee.a = 1;
        temp.b = 5;

        arrpush(sa, temp);
        arrpush(sa, ee);
        asserteq_int(arrpop(sa).a, 1);
        asserteq_int(arrpop(sa).b, 5);
        arrfree(a);
        arrfree(sa);
    }
}

describe(str) {
    it("strcat_test") {
        str s = str_new();
        s = str_cat(s, "BOCCHI_");
        s = str_cat(s, "POG");
        asserteq_str(s, "BOCCHI_POG");
        str_free(s);
    }

    it("str_from") {
        str s = str_from("Disaster");
        asserteq_str(s, "Disaster");
        str s2 = str_format("I: %d", 542);
        asserteq_str(s2, "I: 542");
        str_free(s2);
    }
}

describe(log) {
    it("warn") { warn("warn"); }
}

snow_main();
