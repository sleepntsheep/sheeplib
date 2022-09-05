#include <stdio.h>
#include <assert.h>
#define SHEEP_DYNARRAY_IMPLEMENTATION
#include "../dynarray.h"
#define SHEEP_STR_IMPLEMENTATION
#include "../str.h"
#define it(a) puts("Testing " #a);

int main() {
    it(new array) {
        int *a = arrnew(int);
        assert(a);
    }

    it(array info) {
        int *a = arrnew(int);
        dynarray_info_t *info = dynarray_info(a);
        assert(info);
        assert(DYNARRAY_MIN_CAPACITY == info->capacity);
        assert(0 == info->length);
        assert(sizeof(int) == info->membsize);
    }

    it(array len) {
        int *a = arrnew(int);
        assert(arrlen(a) == 0);
        arrpush(a, 5);
        arrpush(a, 5);
        assert(arrlen(a) == 2);
    }

    it(arrcap_test) {
        int *a = arrnew(int);
        assert(arrcap(a) == 4);
        arrpush(a, 5);
        arrpush(a, 5);
        arrpush(a, 5);
        assert(arrcap(a) == 4);
        arrpush(a, 5);
        assert(arrcap(a) == 4);
        arrpush(a, 5);
        assert(arrcap(a) == 8);
    }

    it(array push) {
        int *a = arrnew(int);
        for (int i = 0; i < 10; i++)
        {
            arrpush(a, i);
            assert(a[i] == i);
        }
    }

    it(array del) {
        int *a = arrnew(int);
        for (int i = 0; i < 5; i++)
            arrpush(a, i);
        arrdel(a, 2);
        assert(a[2] == 3);
        assert(arrlen(a) == 4);
    }

    it(array ins) {
        int *a = arrnew(int);
        for (int i = 0; i < 5; i++)
            if (i != 2)
                arrpush(a, i);
        assert(a[2] == 3);
        assert(arrlen(a) == 4);
        arrins(a, 2, 2);
        assert(a[2] == 2);
        assert(a[3] == 3);
        arrfree(a);
        a = arrnew(int);
        for (int i = 0; i < 5; i++)
            arrpush(a, i);
        assert(a[3] == 3);
        arrins(a, 3, -1);
        assert(a[3] == -1);
        arrins(a, 6, -2);
        assert(a[6] == -2);
        int n = arrlen(a);
        for (int i = n; i < n + 5; i++)
        {
            arrins(a, i, i);
            assert(a[i] == i);
        }
    }

    it(arrsetlen_test) {
        int *a = arrnew(int);
        for (int i = 0; i < 1000; i++)
            arrpush(a, i);
        assert(arrlen(a) == 1000);
        arrsetlen(a, 500);
        assert(arrlen(a) == 500);
        arrpush(a, -1);
        assert(arrlen(a) == 501);
        assert(a[500] == -1);
    } 

    it(arrpop_test) {
        int *a = arrnew(int);
        arrpush(a, 1);
        arrpush(a, 1);
        arrpush(a, 1);
        arrpush(a, 2);
        arrpush(a, -1);
        assert(arrpop(a) == -1);
        assert(arrpop(a) == 2);
        assert(arrpop(a) == 1);
        assert(arrpop(a) == 1);
        assert(arrpop(a) == 1);
        assert(arrpop(a) == 0);
        struct sa {int a, b;};
        struct sa *sa = arrnew(struct sa);
        struct sa temp = {1, 5};
        arrpush(sa, temp);
        arrpush(sa, ((struct sa){1, 5}));
        assert(arrpop(sa).a == 1);
        assert(arrpop(sa).b == 5);
    }

    it(arrmembsize_test) {
        void *a = arrnew(int);
        assert(dynarray_membsize(a) == sizeof(int));
        a = arrnew(long);
        assert(dynarray_membsize(a) == sizeof(long));
        a = arrnew(char);
        assert(dynarray_membsize(a) == sizeof(char));
    }

    it(strnew_test) {
        str s = str_new();
        assert(s.b[0] == '\0');
        assert(s.l == 0);
        assert(s.c == SHEEP_STR_INIT_CAP);
        assert(!strcmp(s.b, ""));
    }

    it(strcat_test) {
        str s = str_new();
        str_cat_cstr(&s, "Hello");
        assert(!strcmp(s.b, "Hello"));
        str_cat_cstr(&s, " World");
        assert(!strcmp(s.b, "Hello World"));
        str s2 = cstr("POG");
        str_cat_str(&s, s2);
        assert(!strcmp(s.b, "Hello WorldPOG"));
        /* undefined behaviour
        str_cat_str(&s, s);
        assert(!strcmp(s.b, "Hello WorldHello World"));
        */
    }

    it(cstr_test) {
        str s = cstr("Disaster");
        assert(!strcmp(s.b, "Disaster"));
    }

    it(str_dup) {
        str s = cstr("String");
        str *s2 = str_dup(s);
        assert(s2);
        assert(!strcmp(s2->b, s.b));
    }

    it(str_aprintf) {
        str *p = str_aprintf("%d, POOG", 500);
        assert(!strcmp(p->b, "500, POOG"));
        assert(p->l == 9);
    }

    it(_Generic str_cat) {
        str s = cstr("String");
        str_cat(&s, " Catted");
        assert(!strcmp(s.b, "String Catted"));
        str_cat(&s, cstr(" Catted"));
        assert(!strcmp(s.b, "String Catted Catted"));
    }
}
