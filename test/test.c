//usr/bin/clang "$0" && exec ./a.out "$@"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define SHEEP_DYNARRAY_IMPLEMENTATION
#include "../dynarray.h"
#define SHEEP_STR_IMPLEMENTATION
#include "../str.h"
#include "../algo.h"
#define SHEEP_SJSON_IMPLEMENTATION
#include "../sjson.h"
#define SHEEP_LOG_IMPLEMENTATION
#include "../log.h"
#define SNOW_ENABLED
#include "snow.h"

/* 
 * TODO 
 *     add more complicated nested sjson.h test cases 
 */

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

describe(dynarray) {
    it("new array") {
        int *a = arrnew(int);
        assert(a);
    }

    it("array len") {
        int *a = arrnew(int);
        asserteq_int(arrlen(a), 0);
        arrpush(a, 5);
        arrpush(a, 5);
        asserteq_int(arrlen(a), 2);
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
    }

    it("array push") {
        int *a = arrnew(int), i;
        for (i = 0; i < 10; i++)
        {
            arrpush(a, i);
            assert(a[i] == i);
        }
    }

    it("array del") {
        int *a = arrnew(int), i;
        for (i = 0; i < 5; i++)
            arrpush(a, i);
        arrdel(a, 2);
        asserteq_int(a[2], 3);
        asserteq_int(arrlen(a), 4);
    }

    it("array ins") {
        int *a = arrnew(int);
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
        a = arrnew(int);
        for (i = 0; i < 5; i++)
            arrpush(a, i);
        asserteq_int(a[3], 3);
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
        arrfree(a);
    }

    it("arrsetlen_test") {
        int *a = arrnew(int), i;
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
        struct sa {int a, b;};
        struct sa ee, temp, *sa;
        int *a = arrnew(int);
        arrpush(a, 1);
        arrpush(a, 1);
        arrpush(a, 1);
        arrpush(a, 2);
        arrpush(a, -1);
        assert(arrpop(a) == -1);
        asserteq_int(arrpop(a), 2);
        asserteq_int(arrpop(a), 1);
        asserteq_int(arrpop(a), 1);
        asserteq_int(arrpop(a), 1);
        asserteq_int(arrpop(a), 0);
        sa = arrnew(struct sa);
        temp.a = 1; temp.b = 5;
        ee.a = 1; temp.b = 5;
        arrpush(sa, temp);
        arrpush(sa, ee);
        asserteq_int(arrpop(sa).a, 1);
        asserteq_int(arrpop(sa).b, 5);
        arrfree(a);
    }

    it("arrmembsize_test") {
        void *a = arrnew(int);
        assert(dynarray_membsize(a) == sizeof(int));
        a = arrnew(long);
        assert(dynarray_membsize(a) == sizeof(long));
        arrfree(a);
        a = arrnew(char);
        assert(dynarray_membsize(a) == sizeof(char));
        arrfree(a);
    }
}

describe(str) {
    it("strnewalloc_test") {
        str s = str_new();
        assert(s.b[0] == '\0');
        asserteq_int(s.l, 0);
        assert(s.c == SHEEP_STR_INIT_CAP);
        asserteq_str(s.b, "");
        free(s.b);
    }

    it("strcat_test") {
        str s = str_new();
        str s2 = str_from_c("POG");
        str_catc(&s, "Hello");
        asserteq_str(s.b, "Hello");
        str_catc(&s, " World");
        asserteq_str(s.b, "Hello World");
        str_cat(&s, s2);
        asserteq_str(s.b, "Hello WorldPOG");
        free(s.b);
        s = str_new();
        for (int i =0 ;i < 10000; i++)
            str_catc(&s, "EEEEEEEEEE");
        asserteq_int(s.l, 100000);
        free(s.b);
        /* undefined behaviour
        str_cat(&s, s);
        asserteq_str(s.b, "Hello WorldHello World");
        */
    }

    it("str_from_c") {
        str s = str_from_c("Disaster");
        asserteq_str(s.b, "Disaster");
    }

    it("str_from_cn") {
        str s = str_from_cn("Disaster", 5);
        asserteq_int(str_cmpc(s, "Disas"), 0);
        asserteq_str(s.b, "Disas");
        asserteq_int(s.l, 5);
    }

    it("str_substr") {
        str s = str_from_c("Paper Machine");
        str sub = str_substr(s, 4, 8);
        asserteq_int(sub.l, 5);
        asserteq_str(sub.b, "r Mac");
        assert(!str_cmpc(sub, "r Mac"));
        free(sub.b);
    } 

    it("str_cmp") {
        str s = str_from_c("Paper Machine");
        asserteq_int(str_cmp(s, s), 0);
        asserteq_int(str_cmp(s, str_from_c("Paper Machine")), 0);
        assert(str_cmpc(s, "AAAAAAAAAAAAAAAA") < 0);
        assert(str_cmpc(s, "ZZZZZZZZZZZZZZZZ") < 0);
        assert(str_cmpc(s, "AAAAAAAAA") > 0);
    }

    it("str_from_copy_c") {
        char *p = malloc(1000);
        strcpy(p, "Dynamic String");
        struct str s = str_from_copy_c(p);
        free(p);
        asserteq_str(s.b, "Dynamic String");
        free(s.b);
        assert(!str_cmpc(s, "Dynamic String"));
    }

    it("str_cmp") {
        struct str s = str_from_c ("POG Duck Duck");
        struct str s2 = str_from_c("Duck POG POG");
        assert(str_cmp(s, s2) > 0);
    }

    it("str_dup") {
        str s = str_from_c("String");
        str *s2 = str_dup(s);
        assert(s2);
        assert(s2->b == s.b);
        asserteq_str(s2->b, s.b);
        free(s2);
    }

    it("str_aprintf") {
        str p = str_aprintf("%d, POOG", 500);
        asserteq_str(p.b, "500, POOG");
        asserteq_int(p.l, 9);
        free(p.b);
    }

    it("str_find_sub") {
        str s = str_from_c("POOOOG");
        asserteq_int(str_find_subc(s, "OOO"), 1);
        asserteq_int(str_find_subc(s, "POOO"), 0);
        str d = str_from_c("ABCDEFEF");
        asserteq_int(str_find_subc(d, "ABCDEFEF"), 0);
        asserteq_int(str_find_subc(d, "ABCDEFE"), 0);
        asserteq_int(str_find_subc(d, "ABCDEF"), 0);
        asserteq_int(str_find_subc(d, "ABCDE"), 0);
        asserteq_int(str_find_subc(d, "ABCD"), 0);
        asserteq_int(str_find_subc(d, "ABC"), 0);
        asserteq_int(str_find_subc(d, "AB"), 0);
        asserteq_int(str_find_subc(d, "A"), 0);
        asserteq_int(str_find_subc(d, "BCDEFEF"), 1);
        asserteq_int(str_find_subc(d, "BCDEFE"), 1);
        asserteq_int(str_find_subc(d, "BCDEF"), 1);
        asserteq_int(str_find_subc(d, "BCDE"), 1);
        asserteq_int(str_find_subc(d, "BCD"), 1);
        asserteq_int(str_find_subc(d, "BC"), 1);
        asserteq_int(str_find_subc(d, "B"), 1);
        asserteq_int(str_find_subc(d, "CDEFEF"), 2);
        asserteq_int(str_find_subc(d, "CDEFE"), 2);
        asserteq_int(str_find_subc(d, "CDEF"), 2);
        asserteq_int(str_find_subc(d, "CDE"), 2);
        asserteq_int(str_find_subc(d, "CD"), 2);
        asserteq_int(str_find_subc(d, "C"), 2);
        asserteq_int(str_find_subc(d, "DEFEF"), 3);
        asserteq_int(str_find_subc(d, "DEFE"), 3);
        asserteq_int(str_find_subc(d, "DEF"), 3);
        asserteq_int(str_find_subc(d, "DE"), 3);
        asserteq_int(str_find_subc(d, "D"), 3);
        asserteq_int(str_find_subc(d, "EFEF"), 4);
        asserteq_int(str_find_subc(d, "EFE"), 4);
        asserteq_int(str_find_subc(d, "EF"), 4);
        asserteq_int(str_find_subc(d, "E"), 4);
        asserteq_int(str_find_subc(d, "FEF"), 5);
        assert(str_find_subc(d, "Z") == -1);
    }

    it("str_split") {
        str h = str_from_c("Welcome\nTo\nFarm");
        str n = str_from_c("\n");
        struct strarray sp = str_split(h, n);
        asserteq_int(sp.l, 3);
        assert(!str_cmpc(sp.a[0], "Welcome"));
        assert(!str_cmpc(sp.a[1], "To"));
        assert(!str_cmpc(sp.a[2], "Farm"));
        strarray_free(&sp);
    }
}

describe(algo) {
    size_t j, n = 100000;
    srand(time(NULL));
    int *a = rndarr(n);
    it("ssort") {
        ssort_int(a, n);
        for (j = 0; j < n - 1; j++)
            assert(a[j] <= a[j+1]);
    }
    arrfree(a);

    a = arrnew(int);
    for (int i = 0; i < n; i++)
        arrpush(a, i);
    it("sbsearch") {
        for (size_t i = 0; i < n; i++) {
            int *br = sbsearch_int(i, a, n);
            assert(*br == i);
            assert(br - a == i);
        }
        assert(sbsearch_int(-1, a, n) == NULL);
    }
    it("supperbound") {
        for (size_t i = 0; i < n - 1; i++) {
            int *up = supperbound_int(i, a, n);
            assert(*up > i);
            assert(up - a == i + 1);
        }
        assert(supperbound_int(n, a, n) == NULL);
    }
    it("slowerbound") {
        for (size_t i = 0; i < n; i++) {
            int *low = slowerbound_int(i, a, n);
            assert(*low == i);
            assert(low - a == i);
        }
        assert(slowerbound_int(n+1, a, n) == NULL);
    }
    arrfree(a);
}

describe(json) {

    char *dup;
    sjson_register_logger(printf);

    it("parse null") {
        sjsonlexer lexer;
        sjsonlexer_init(&lexer, "null", 4);
        sjsonlexer_lex(&lexer);
        sjson *j = sjson_parse(&lexer.toks);
        asserteq_int(j->type, SJSON_NULL);
        sjsonlexer_init(&lexer, "\"\\n\"", 3);
        sjsonlexer_lex(&lexer);
        j = sjson_parse(&lexer.toks);
        asserteq_str(j->v.str, "\n");
    }

    it("parse true") {
        sjson *j = sjson_serialize("true", 4);
        asserteq_int(j->type, SJSON_TRUE);
        sjson_free(j);
    }

    it("parse false") {
        sjson *j = sjson_serialize("false", 5);
        asserteq_int(j->type, SJSON_FALSE);
        sjson_free(j);
    }

    it("parse positive int") {
        const char *nums[] = { "2", "3", "4", "5", "6", "10000" };
        const int ans[] = { 2, 3, 4, 5, 6, 10000 };
        for (int i = 0; i < sizeof nums / sizeof *nums; i++) {
            sjson *j = sjson_serialize(nums[i], strlen(nums[i]));
            asserteq_int(j->type, SJSON_NUMBER);
            asserteq_dbl(j->v.num, ans[i]);
            sjson_free(j);
        }
    }

    it("parse negative int") {
        const char *nums[] = { "-2", "-3", "-4", "-5", "-6", "-10000" };
        const int ans[] = { -2, -3, -4, -5, -6, -10000 };
        for (int i = 0; i < sizeof nums / sizeof *nums; i++) {
            sjson *j = sjson_serialize(nums[i], strlen(nums[i]));
            asserteq_int(j->type, SJSON_NUMBER);
            asserteq_dbl(j->v.num, ans[i]);
            sjson_free(j);
        }
    }

    it("parse zero") {
        const char *nums[] = { "0", "00", "000", "0000", "00000" };
        for (int i = 0; i < sizeof nums / sizeof *nums; i++) {
            dup = strdup(nums[i]);
            sjson *j = sjson_serialize(dup, strlen(dup));
            asserteq_int(j->type, SJSON_NUMBER);
            asserteq_dbl(j->v.num, 0);
            free(dup);
        }
    }

    it("parse positive floating") {
        const char *nums[] = { "2.213", "3.9203", "4.23", "5.738", "6.01", "10000.389" };
        const double ans[] = { 2.213, 3.9203, 4.23, 5.738, 6.01, 10000.389 };
        for (int i = 0; i < sizeof nums / sizeof *nums; i++) {
            dup = strdup(nums[i]);
            sjson *j = sjson_serialize(dup, strlen(dup));
            asserteq_int(j->type, SJSON_NUMBER);
            asserteq_dbl(j->v.num, ans[i]);
            free(dup);
        }
    }

    it("parse negative floating") {
        const char *nums[] = { "-2.213", "-3.9203", "-4.23", "-5.738", "-6.01", "-10000.389" };
        const double ans[] = { -2.213, -3.9203, -4.23, -5.738, -6.01, -10000.389 };
        for (int i = 0; i < sizeof nums / sizeof *nums; i++) {
            dup = strdup(nums[i]);
            sjson *j = sjson_serialize(dup, strlen(dup));
            asserteq_int(j->type, SJSON_NUMBER);
            asserteq_dbl(j->v.num, ans[i]);
            free(dup);
        }
    }

    it("parse string") {
        char *strs[] = { "\"POOOG\"", "\"SHIEN\"", "\"WATER\"", "\"Kronii\"" };
        char *ans[] = { "POOOG", "SHIEN", "WATER", "Kronii" };
        for (int i = 0; i < sizeof strs / sizeof *strs; i++) {
            dup = strdup(strs[i]);
            sjson *j = sjson_serialize(dup, strlen(dup));
            asserteq_int(j->type, SJSON_STRING);
            asserteq_str(j->v.str, ans[i]);
            sjson_free(j);
            free(dup);
        }
    }

    it("parse escape code") {
        char *strs[] = { "\"\\n\"", "\"\\r\"", "\"QE 2 is dedge\\t\"" };
        char *ans[] = { "\n", "\r", "QE 2 is dedge\t" };
        for (int i = 0; i < sizeof strs / sizeof *strs; i++) {
            sjson *j = sjson_serialize(strs[i], strlen(strs[i]));
            asserteq_int(j->type, SJSON_STRING);
            asserteq_str(j->v.str, ans[i]);
            sjson_free(j);
        }
    }

    it("parse escape code \\u unicode") {
        char *strs[] = { "\"\\u3FA3\"", "\"\\u4712\"" };
        char *ans[] = { "\x3f\xa3", "\x47\x12" };
        for (int i = 0; i < sizeof strs / sizeof *strs; i++) {
            dup = strdup(strs[i]);
            sjson *j = sjson_serialize(dup, strlen(dup));
            asserteq_int(j->type, SJSON_STRING);
            asserteq_str(j->v.str, ans[i]);
            sjson_free(j);
            free(dup);
        }
    }

    it("parse empty array") {
        sjson *j = sjson_serialize("[]", 2);
        assert(j);
        asserteq_int(j->type, SJSON_ARRAY);
        assert(!j->v.child);
    }

    it("parse non empty array") {
        sjson *j = sjson_serialize("[\"a\",\"b\"]", 9);
        assert(j);
        asserteq_int(j->type, SJSON_ARRAY);
        assert(j->v.child);
        asserteq_int(j->v.child->type, SJSON_STRING);
        assert(j->v.child->next);
    }

    it("parse nested array") {
        sjson *j = sjson_serialize("[[[]]]", 6);
        assert(j);
        asserteq_int(j->type, SJSON_ARRAY);
        asserteq_int(j->v.child->type, SJSON_ARRAY);
        asserteq_ptr(j->v.child->next, 0);
        asserteq_ptr(j->v.child->prev, 0);
        asserteq_int(j->v.child->v.child->type, SJSON_ARRAY);
    }

    it("parse int array") {
        char *s = "[1,2,3,4,5,6,7,8]";
        sjson *j = sjson_serialize(s, strlen(s));
        assert(j);
        assert(j->v.child);
        assert(j->v.tail);
        assertneq_ptr(j->v.tail, j->v.child);
        asserteq_int(j->v.child->type, SJSON_NUMBER);
        asserteq_int(j->v.child->v.num, 1);
        asserteq_int(j->v.child->next->v.num, 2);
        asserteq_int(j->v.child->next->next->v.num, 3);
        asserteq_int(j->v.child->next->next->next->v.num, 4);
        asserteq_int(j->v.child->next->next->next->next->v.num, 5);
        asserteq_int(j->v.child->next->next->next->next->next->v.num, 6);
        asserteq_int(j->v.child->next->next->next->next->next->next->v.num, 7);
        asserteq_int(j->v.child->next->next->next->next->next->next->next->v.num, 8);
    }

    it("parse mixed array") {
        char *s = "[1, \"EEEE\"]";
        sjson *j = sjson_serialize(s, strlen(s));
        assert(j);
        assert(j->v.child);
        assert(j->v.tail);
        assertneq_ptr(j->v.tail, j->v.child);
        asserteq_str(j->v.tail->v.str, "EEEE");
        asserteq_int(j->v.child->v.num, 1);
    }

    it("parse empty object") {
        char *s = strdup("{}");
        sjson *j = sjson_serialize(s, strlen(s));
        asserteq_int(j->type, SJSON_OBJECT);
        asserteq_ptr(j->v.child, 0);
    }

    it("parse non empty object") {
        char *j;
        j = "{ \"key\": \"value\", \"num\": -2839.489 } ";
        sjson *json = sjson_serialize(j, strlen(j));
        assert(json);
        asserteq_int(json->type, SJSON_OBJECT);
        assertneq_ptr(json->v.child, NULL);
        asserteq_str(json->v.child->key, "key");;
        asserteq_str(json->v.child->v.str, "value");;
        asserteq_str(json->v.child->next->key, "num");
        assert(json->v.child->next->v.num == -2839.489);
        sjson_free(json);
    }

    it("parse nested object") {
        char *j;
        j = "{ \"key\": \"value\", \"nest\": {} } ";
        sjson *json = sjson_serialize(j, strlen(j));
        assert(json);
        asserteq_int(json->type, SJSON_OBJECT);
        assertneq_ptr(json->v.child, NULL);
        asserteq_str(json->v.child->key, "key");;
        asserteq_str(json->v.child->v.str, "value");;
        asserteq_str(json->v.child->next->key, "nest");
        asserteq_int(json->v.child->next->type, SJSON_OBJECT);
        asserteq_ptr(json->v.child->next->v.child, NULL);
        sjson_free(json);
    }

}

describe(log) {
    it("warn") {
        warn("warn");
    }
}

snow_main();
