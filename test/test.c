#include <stdio.h>
#include "snow.h"
#define SHEEP_DYNARRAY_IMPLEMENTATION
#include "../dynarray.h"
#define SHEEP_STR_IMPLEMENTATION
#include "../str.h"
#define SHEEP_SMATH_IMPLEMENTATION
#include "../smath.h"

describe(dynarray) {
    it("new array") {
        int *a = arrnew(int);
        assert(a);
        defer(arrfree(a));
    }

    it("array info") {
        int *a = arrnew(int);
        dynarray_info_t *info = dynarray_info(a);
        assert(info);
        asserteq_int(DYNARRAY_MIN_CAPACITY, info->capacity);
        asserteq_int(0, info->length);
        asserteq_int(sizeof(int), info->membsize);
        defer(arrfree(a));
    }

    it("array len") {
        int *a = arrnew(int);
        asserteq_int(arrlen(a), 0);
        arrpush(a, 5);
        arrpush(a, 5);
        asserteq_int(arrlen(a), 2);
        defer(arrfree(a));
    }

    it("arrcap_test") {
        int *a = arrnew(int);
        asserteq_int(arrcap(a), 4);
        arrpush(a, 5);
        arrpush(a, 5);
        arrpush(a, 5);
        asserteq_int(arrcap(a), 4);
        arrpush(a, 5);
        asserteq_int(arrcap(a), 4);
        arrpush(a, 5);
        asserteq_int(arrcap(a), 8);
        defer(arrfree(a));
    }

    it("array push") {
        int *a = arrnew(int);
        for (int i = 0; i < 10; i++)
        {
            arrpush(a, i);
            asserteq_int(a[i], i);
        }
        defer(arrfree(a));
    }

    it("array del") {
        int *a = arrnew(int);
        for (int i = 0; i < 5; i++)
            arrpush(a, i);
        arrdel(a, 2);
        asserteq_int(a[2], 3);
        asserteq_int(arrlen(a), 4);
        defer(arrfree(a));
    }


    it("array ins") {
        int *a = arrnew(int);
        for (int i = 0; i < 5; i++)
            if (i != 2)
                arrpush(a, i);
        asserteq_int(a[2], 3);
        asserteq_int(arrlen(a), 4);
        arrins(a, 2, 2);
        asserteq_int(a[2], 2);
        asserteq_int(a[3], 3);
        arrfree(a);
        a = arrnew(int);
        for (int i = 0; i < 5; i++)
            arrpush(a, i);
        asserteq_int(a[3], 3);
        arrins(a, 3, -1);
        asserteq_int(a[3], -1);
        arrins(a, 6, -2);
        asserteq_int(a[6], -2);
        int n = arrlen(a);
        for (int i = n; i < n + 5; i++)
        {
            arrins(a, i, i);
            asserteq_int(a[i], i);
        }
        defer(arrfree(a));
    }

    it("arrsetlen_test") {
        int *a = arrnew(int);
        for (int i = 0; i < 1000; i++)
            arrpush(a, i);
        asserteq_int(arrlen(a), 1000);
        arrsetlen(a, 500);
        asserteq_int(arrlen(a), 500);
        arrpush(a, -1);
        asserteq_int(arrlen(a), 501);
        asserteq_int(a[500], -1);
        defer(arrfree(a));
    } 

    it("arrpop_test") {
        int *a = arrnew(int);
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
        asserteq_int(arrpop(a), 0);
        struct sa {int a, b;};
        struct sa *sa = arrnew(struct sa);
        struct sa temp = {1, 5};
        arrpush(sa, temp);
        arrpush(sa, ((struct sa){1, 5}));
        assert(arrpop(sa).a == 1);
        assert(arrpop(sa).b == 5);
        defer(arrfree(a));
        defer(arrfree(sa));
    }

    it("arrmembsize_test") {
        void *a = arrnew(int);
        asserteq_int(dynarray_membsize(a), sizeof(int));
        a = arrnew(long);
        asserteq_int(dynarray_membsize(a), sizeof(long));
        a = arrnew(char);
        asserteq_int(dynarray_membsize(a), sizeof(char));
        defer(arrfree(a));
    }
}

describe(str) {
    it("strnew_test") {
        str s = str_new();
        asserteq_int(s.b[0], '\0');
        asserteq_int(s.l, 0);
        asserteq_int(s.c, SHEEP_STR_INIT_CAP);
        asserteq_str(s.b, "");
        defer(free(s.b));
    }

    it("strcat_test") {
        str s = str_new();
        str_cat_cstr(&s, "Hello");
        asserteq_str(s.b, "Hello");
        str_cat_cstr(&s, " World");
        asserteq_str(s.b, "Hello World");
        str s2 = cstr("POG");
        str_cat_str(&s, s2);
        asserteq_str(s.b, "Hello WorldPOG");
        /* undefined behaviour
        str_cat_str(&s, s);
        asserteq_str(s.b, "Hello WorldHello World");
        */
    }

    it("cstr_test") {
        str s = cstr("Disaster");
        asserteq_str(s.b, "Disaster");
    }

    it("str_dup") {
        str s = cstr("String");
        str *s2 = str_dup(s);
        assert(s2);
        asserteq_str(s2->b, s.b);
    }

    it("str_aprintf") {
        str *p = str_aprintf("%d, POOG", 500);
        asserteq_str(p->b, "500, POOG");
        asserteq_int(p->l, 9);
    }

    it("_Generic str_cat") {
        str s = cstr("String");
        str_cat(&s, " Catted");
        asserteq_str(s.b, "String Catted");
        str_cat(&s, cstr(" Catted"));
        asserteq_str(s.b, "String Catted Catted");
    }
}

describe(smath) {
    it("mini maxi") {
        asserteq_int(sheep_mini(5, 9), 5);
        asserteq_int(sheep_maxi(5, 9), 9);
        asserteq_int(sheep_mini(5, -9), -9);
        asserteq_int(sheep_maxi(5, -9), 5);
    }

    it("minlf maxlf") {
        asserteq_dbl(sheep_minlf(5.4L, 2.0L), 2.0L);
        asserteq_dbl(sheep_minlf(5L, 1.0L/0.0L), 5L);
        asserteq_dbl(sheep_maxlf(-.159, -.158), -.158);
    }

    it("minu maxu") {
        asserteq_uint(sheep_minu(0, -1), 0);
        asserteq_uint(sheep_maxu(0, 3000000000), 3000000000);
    }
}

snow_main();
