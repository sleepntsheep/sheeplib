//usr/bin/clang "$0" && exec ./a.out "$@"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define SHEEP_DYNARRAY_IMPLEMENTATION
#include "../include/dynarray.h"
#define SHEEP_STR_IMPLEMENTATION
#include "../include/str.h"
#include "../include/algo.h"
#define SHEEP_SJSON_IMPLEMENTATION
#include "../include/sjson.h"


#undef assert
#define assert(chk) \
    do { \
        if (!(chk)) \
            fprintf(stderr, "Assertion (%s) failed %s at line %d \n ", #chk, __FILE__,__LINE__); \
    } while (0)

clock_t start;

#define it(a) \
    start = clock(); \
    printf("%s \n\t", #a); \
    fflush(stdout);

#define ti \
    printf("time taken %ld (cpu time)\n", (clock() - start));

int *rndarr(int size) {
    int i;
    int *a = arrnew(int);
    for (i = 0; i < arrlen(a); i++)
         arrpop(a);
    for (i = 0; i < size; i++) {
        int b = rand() % 65531;
        arrpush(a, b);
    }
    return a;
}

int cmpm(int a, int b) {
	return a-b;
}

SALGO_DECL(int,cmpm)

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

    it(strnewalloc_test) {
        str s = str_new();
        assert(s.b[0] == '\0');
        assert(s.l == 0);
        assert(s.c == SHEEP_STR_INIT_CAP);
        assert(!strcmp(s.b, ""));
    } ti

    it(strcat_test) {
        str s = str_new();
        str s2 = str_from_c("POG");
        str_catc(&s, "Hello");
        assert(!strcmp(s.b, "Hello"));
        str_catc(&s, " World");
        assert(!strcmp(s.b, "Hello World"));
        str_cat(&s, s2);
        assert(!strcmp(s.b, "Hello WorldPOG"));
        /* undefined behaviour
        str_cat(&s, s);
        assert(!strcmp(s.b, "Hello WorldHello World"));
        */
    } ti

    it(str_from_c) {
        str s = str_from_c("Disaster");
        assert(!strcmp(s.b, "Disaster"));
    } ti

    it(str_from_cn) {
        str s = str_from_cn("Disaster", 5);
        assert(str_cmpc(s, "Disas") == 0);
        assert(!strcmp(s.b, "Disas"));
        assert(s.l == 5);
    } ti

    it (str_substr) {
        str s = str_from_c("Paper Machine");
        str sub = str_substr(s, 4, 8);
        assert(sub.l == 5);
        assert(!strcmp(sub.b, "r Mac"));
        assert(!str_cmpc(sub, "r Mac"));
    } ti 

    it (str_cmp) {
        str s = str_from_c("Paper Machine");
        assert(str_cmp(s, s) == 0);
        assert(str_cmp(s, str_from_c("Paper Machine")) == 0);
        assert(str_cmpc(s, "AAAAAAAAAAAAAAAA") < 0);
        assert(str_cmpc(s, "ZZZZZZZZZZZZZZZZ") < 0);
        assert(str_cmpc(s, "AAAAAAAAA") > 0);
    } ti

    it (str_from_copy_c) {
        char *p = malloc(1000);
        strcpy(p, "Dynamic String");
        struct str s = str_from_copy_c(p);
        assert(!strcmp(s.b, "Dynamic String"));
        assert(!str_cmpc(str_from_c(p), "Dynamic String"));
        free(p);
        assert(!str_cmpc(s, "Dynamic String"));
    } ti

    it (str_cmp) {
        struct str s = str_from_c ("POG Duck Duck");
        struct str s2 = str_from_c("Duck POG POG");
        assert(str_cmp(s, s2) > 0);
    } ti

    it(str_dup) {
        str s = str_from_c("String");
        str *s2 = str_dup(s);
        assert(s2);
        assert(s2->b == s.b);
        assert(!strcmp(s2->b, s.b));
    } ti

    it(str_aprintf) {
        str *p = str_aprintf("%d, POOG", 500);
        assert(!strcmp(p->b, "500, POOG"));
        assert(p->l == 9);
    } ti

    it(str_find_sub) {
        str s = str_from_c("POOOOG");
        assert(str_find_subc(s, "OOO") == 1);
        assert(str_find_subc(s, "POOO") == 0);
        str d = str_from_c("ABCDEFEF");
        assert(str_find_subc(d, "ABCDEFEF") == 0);
        assert(str_find_subc(d, "ABCDEFE") == 0);
        assert(str_find_subc(d, "ABCDEF") == 0);
        assert(str_find_subc(d, "ABCDE") == 0);
        assert(str_find_subc(d, "ABCD") == 0);
        assert(str_find_subc(d, "ABC") == 0);
        assert(str_find_subc(d, "AB") == 0);
        assert(str_find_subc(d, "A") == 0);
        assert(str_find_subc(d, "BCDEFEF") == 1);
        assert(str_find_subc(d, "BCDEFE") == 1);
        assert(str_find_subc(d, "BCDEF") == 1);
        assert(str_find_subc(d, "BCDE") == 1);
        assert(str_find_subc(d, "BCD") == 1);
        assert(str_find_subc(d, "BC") == 1);
        assert(str_find_subc(d, "B") == 1);
        assert(str_find_subc(d, "CDEFEF") == 2);
        assert(str_find_subc(d, "CDEFE") == 2);
        assert(str_find_subc(d, "CDEF") == 2);
        assert(str_find_subc(d, "CDE") == 2);
        assert(str_find_subc(d, "CD") == 2);
        assert(str_find_subc(d, "C") == 2);
        assert(str_find_subc(d, "DEFEF") == 3);
        assert(str_find_subc(d, "DEFE") == 3);
        assert(str_find_subc(d, "DEF") == 3);
        assert(str_find_subc(d, "DE") == 3);
        assert(str_find_subc(d, "D") == 3);
        assert(str_find_subc(d, "EFEF") == 4);
        assert(str_find_subc(d, "EFE") == 4);
        assert(str_find_subc(d, "EF") == 4);
        assert(str_find_subc(d, "E") == 4);
        assert(str_find_subc(d, "FEF") == 5);
        assert(str_find_subc(d, "Z") == -1);
    } ti

    it(str_split) {
        str h = str_from_c("Welcome\nTo\nFarm");
        str n = str_from_c("\n");
        struct strarray sp = str_split(h, n);
        assert(sp.l == 3);
        assert(!str_cmpc(sp.a[0], "Welcome"));
        assert(!str_cmpc(sp.a[1], "To"));
        assert(!str_cmpc(sp.a[2], "Farm"));
    } ti

    /* mustn't take no more than O(nlogn) */
    {
        size_t j, n = 1000000;
        srand(time(NULL));
        int *a = rndarr(n);
        it(ssort) {
            ssort_int(a, n);
            for (j = 0; j < n - 1; j++) {
                assert(a[j] <= a[j+1]);
            }
        } ti
    }

    {
        int n = 1000000, i;
        int *a = arrnew(int);
        for (i = 0; i < n; i++)
            arrpush(a, i);
        it(sbsearch) {
            for (size_t i = 0; i < n; i++) {
                int *br = sbsearch_int(i, a, n);
                assert(*br == i);
                assert(br - a == i);
            }
        } ti
        i = -1;
        assert(sbsearch_int(i, a, n) == NULL);
        it(supperbound) {
            for (size_t i = 0; i < n - 1; i++) {
                int *up = supperbound_int(i, a, n);
                assert(*up > i);
                assert(up - a == i + 1);
            }
        } ti
        i = 2e9;
        assert(supperbound_int(i, a, n) == NULL);
        it(slowerbound) {
            for (size_t i = 0; i < n; i++) {
                int *low = slowerbound_int(i, a, n);
                assert(*low == i);
                assert(low - a == i);
            }
        } ti
        assert(slowerbound_int(i, a, n) == NULL);
    }

    it(json) {
        char *j;
        j = "{ \"key\": \"value\", \"num\": -2839.489 } ";
        sjsontokarr *toks = sjson_lex(strdup(j));
        sjson *json = sjson_parse(toks);
        assert(!strcmp(json->childvalue->key, "key"));;
        assert(!strcmp(json->childvalue->stringvalue, "value"));;
        assert(!strcmp(json->childvalue->next->key, "num"));
        assert(json->childvalue->next->numbervalue == -2839.489);
    } ti
}
