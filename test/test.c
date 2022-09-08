#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define SHEEP_DYNARRAY_IMPLEMENTATION
#include "../dynarray.h"
#define SHEEP_STR_IMPLEMENTATION
#include "../str.h"
#define SHEEP_SSORT_IMPLEMENTATION
#include "../ssort.h"
#undef assert
#define assert(chk) \
    do \
        if (!(chk)) \
            fprintf(stderr, "Assertion (%s) failed %s at line %d \n ", #chk, __FILE__,__LINE__); \
    while (0)

clock_t start;

#define it(a) \
    start = clock(); \
    printf("%s \t\t\t", #a); \

#define ti \
    printf("time taken %ld (cpu time)\n", (clock() - start));

int *a;

void rndarr(int size) {
    int i;
    for (i = 0; i < arrlen(a); i++)
        arrpop(a);
    for (i = 0; i < size; i++) {
        int b = rand() % 65531;
        arrpush(a, b);
    }
}

int cmpfnc(const void *a, const void *b) {
	return *(int*)a - *(int*)b;
}

int main() {
    it(new array) {
        int *a = arrnew(int);
        assert(a);
    } ti

    it(array info) {
        int *a = arrnew(int);
        dynarray_info_t *info = dynarray_info(a);
        assert(info);
        assert(DYNARRAY_MIN_CAPACITY == info->capacity);
        assert(0 == info->length);
        assert(sizeof(int) == info->membsize);
    } ti

    it(array len) {
        int *a = arrnew(int);
        assert(arrlen(a) == 0);
        arrpush(a, 5);
        arrpush(a, 5);
        assert(arrlen(a) == 2);
    } ti

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
    } ti

    it(array push) {
        int *a = arrnew(int), i;
        for (i = 0; i < 10; i++)
        {
            arrpush(a, i);
            assert(a[i] == i);
        }
    } ti

    it(array del) {
        int *a = arrnew(int), i;
        for (i = 0; i < 5; i++)
            arrpush(a, i);
        arrdel(a, 2);
        assert(a[2] == 3);
        assert(arrlen(a) == 4);
    } ti

    it(array ins) {
        int *a = arrnew(int);
        int i, n;
        for (i = 0; i < 5; i++)
            if (i != 2)
                arrpush(a, i);
        assert(a[2] == 3);
        assert(arrlen(a) == 4);
        arrins(a, 2, 2);
        assert(a[2] == 2);
        assert(a[3] == 3);
        arrfree(a);
        a = arrnew(int);
        for (i = 0; i < 5; i++)
            arrpush(a, i);
        assert(a[3] == 3);
        arrins(a, 3, -1);
        assert(a[3] == -1);
        arrins(a, 6, -2);
        assert(a[6] == -2);
        n = arrlen(a);
        for (i = n; i < n + 5; i++)
        {
            arrins(a, i, i);
            assert(a[i] == i);
        }
    } ti

    it(arrsetlen_test) {
        int *a = arrnew(int), i;
        for (i = 0; i < 1000; i++)
            arrpush(a, i);
        assert(arrlen(a) == 1000);
        arrsetlen(a, 500);
        assert(arrlen(a) == 500);
        arrpush(a, -1);
        assert(arrlen(a) == 501);
        assert(a[500] == -1);
    } ti

    it(arrpop_test) {
        struct sa {int a, b;};
        struct sa ee, temp, *sa;
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
        sa = arrnew(struct sa);
        temp.a = 1; temp.b = 5;
        ee.a = 1; temp.b = 5;
        arrpush(sa, temp);
        arrpush(sa, ee);
        assert(arrpop(sa).a == 1);
        assert(arrpop(sa).b == 5);
    } ti

    it(arrmembsize_test) {
        void *a = arrnew(int);
        assert(dynarray_membsize(a) == sizeof(int));
        a = arrnew(long);
        assert(dynarray_membsize(a) == sizeof(long));
        a = arrnew(char);
        assert(dynarray_membsize(a) == sizeof(char));
    } ti

    it(strnew_test) {
        str s = str_new();
        assert(s.b[0] == '\0');
        assert(s.l == 0);
        assert(s.c == SHEEP_STR_INIT_CAP);
        assert(!strcmp(s.b, ""));
    } ti

    it(strcat_test) {
        str s = str_new();
        str s2 = cstr("POG");
        str_cat_cstr(&s, "Hello");
        assert(!strcmp(s.b, "Hello"));
        str_cat_cstr(&s, " World");
        assert(!strcmp(s.b, "Hello World"));
        str_cat_str(&s, s2);
        assert(!strcmp(s.b, "Hello WorldPOG"));
        /* undefined behaviour
        str_cat_str(&s, s);
        assert(!strcmp(s.b, "Hello WorldHello World"));
        */
    } ti

    it(cstr_test) {
        str s = cstr("Disaster");
        assert(!strcmp(s.b, "Disaster"));
    } ti

    it(str_dup) {
        str s = cstr("String");
        str *s2 = str_dup(s);
        assert(s2);
        assert(!strcmp(s2->b, s.b));
    } ti

    it(str_aprintf) {
        str *p = str_aprintf("%d, POOG", 500);
        assert(!strcmp(p->b, "500, POOG"));
        assert(p->l == 9);
    } ti

#if __STD_C_VERSION__ >= 201112L
    it(_Generic str_cat) {
        str s = cstr("String");
        str_cat(&s, " Catted");
        assert(!strcmp(s.b, "String Catted"));
        str_cat(&s, cstr(" Catted"));
        assert(!strcmp(s.b, "String Catted Catted"));
    } ti
#endif

    /* mustn't take no more than O(nlogn) */
	it(qsort) {
        int i, j;
        srand(time(NULL));
        for (i = 1; i < 3000; i++) {
            a = arrnew(int);
            rndarr(i);
            sheep_qsort(a, i, sizeof *a, cmpfnc);
            for (j = 0; j < i - 1; j++)
                assert(a[j] <= a[j+1]);
        }
	} ti
}
