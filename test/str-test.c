#define SHEEP_STR_IMPLEMENTATION
#define SHEEP_DYNARRAY_IMPLEMENTATION
#include "../dynarray.h"
#include "../str.h"
#include <stdio.h>
#include <check.h>

START_TEST(strnew_test)
{
    str s = str_new();
    ck_assert_int_eq(s.b[0], '\0');
    ck_assert_int_eq(s.l, 0);
    ck_assert_int_eq(s.c, SHEEP_STR_INIT_CAP);
    ck_assert_str_eq(s.b, "");
    free(s.b);
}
END_TEST

int main()
{
    Suite *s1 = suite_create("Str");
    TCase *tc = tcase_create("Str");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc);
    tcase_add_test(tc, strnew_test);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf != 0;
}

