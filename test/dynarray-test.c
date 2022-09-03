#define SHEEP_DYNARRAY_IMPLEMENTATION
#include "../dynarray.h"
#include <stdio.h>
#include <check.h>

START_TEST(arrcreate_test)
{
    int *a = arrnew(int);
    ck_assert_ptr_nonnull(a);
    arrfree(a);
}
END_TEST

START_TEST(arrinfo_test)
{
    int *a = arrnew(int);
    dynarray_info_t *info = dynarray_info(a);
    ck_assert_ptr_nonnull(info);
    ck_assert_int_eq(DYNARRAY_MIN_CAPACITY, info->capacity);
    ck_assert_int_eq(0, info->length);
    ck_assert_int_eq(sizeof(int), info->membsize);
}

START_TEST(arrlen_test)
{
    int *a = arrnew(int);
    ck_assert_int_eq(arrlen(a), 0);
    arrpush(a, 5);
    arrpush(a, 5);
    ck_assert_int_eq(arrlen(a), 2);
    arrfree(a);
}
END_TEST

START_TEST(arrcap_test)
{
    int *a = arrnew(int);
    ck_assert_int_eq(arrcap(a), 4);
    arrpush(a, 5);
    arrpush(a, 5);
    arrpush(a, 5);
    ck_assert_int_eq(arrcap(a), 4);
    arrpush(a, 5);
    ck_assert_int_eq(arrcap(a), 4);
    arrpush(a, 5);
    ck_assert_int_gt(arrcap(a), 4);
    arrfree(a);
}
END_TEST

START_TEST(arrpush_test)
{
    int *a = arrnew(int);
    for (int i = 0; i < 10; i++)
    {
        arrpush(a, i);
        ck_assert_int_eq(a[i], i);
    }
    arrfree(a);
}
END_TEST

START_TEST(arrdel_test)
{
    int *a = arrnew(int);
    for (int i = 0; i < 5; i++)
        arrpush(a, i);
    arrdel(a, 2);
    ck_assert_int_eq(a[2], 3);
    ck_assert_int_eq(arrlen(a), 4);
    arrfree(a);
}

START_TEST(arrins_test)
{
    int *a = arrnew(int);
    for (int i = 0; i < 5; i++)
        if (i != 2)
            arrpush(a, i);
    ck_assert_int_eq(a[2], 3);
    ck_assert_int_eq(arrlen(a), 4);
    arrins(a, 2, 2);
    ck_assert_int_eq(a[2], 2);
    ck_assert_int_eq(a[3], 3);
    arrfree(a);
    a = arrnew(int);
    for (int i = 0; i < 5; i++)
        arrins(a, 0, i); // 4 3 2 1 0
    ck_assert_int_eq(a[1], 3);
}

int main()
{
    Suite *s1 = suite_create("Core");
    TCase *tc = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc);
    tcase_add_test(tc, arrcreate_test);
    tcase_add_test(tc, arrpush_test);
    tcase_add_test(tc, arrlen_test);
    tcase_add_test(tc, arrcap_test);
    tcase_add_test(tc, arrinfo_test);
    tcase_add_test(tc, arrdel_test);
    tcase_add_test(tc, arrins_test);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf != 0;
}

