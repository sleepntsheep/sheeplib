#define SHEEP_DYNARRAY_IMPLEMENTATION
#include "../dynarray.h"
#include <assert.h>

int main() {
    int *a;

    {
        a = arrnew;
        arrpush(a, 5);
        arrpush(a, 5);
        assert(arrlen(a) == 2);
        arrfree(a);
    }

    {
        a = arrnew;
        arrpush(a, 5);
        arrpush(a, 5);
        assert(arrcap(a) == 4);
        arrpush(a, 5);
        arrpush(a, 5);
        arrpush(a, 5);
        assert(arrcap(a) == 8);
        arrfree(a);
    }
    {
        a = arrnew;
        for (int i = 0; i < 10; i++) {
            arrpush(a, i);
            assert(a[i] == i);
        }
        arrfree(a);
    }

    {
        a = arrnew;
        for (int i = 0; i < 5; i++)
            arrpush(a, i);
        arrdel(a, 2);
        assert(a[2] == 3);
        assert(arrlen(a) == 4);
        arrdel(a, 0);
        assert(a[0] == 1);
        arrfree(a);
    }
    {
        a = arrnew;
        for (int i = 0; i < 5; i++)
            if (i != 2)
                arrpush(a, i);
        assert(a[2] == 3);
        assert(arrlen(a) == 4);
        arrins(a, 2, 2);
        assert(a[2] == 2);
        assert(a[3] == 3);
        arrfree(a);
        a = arrnew;
        for (int i = 0; i < 5; i++)
            arrpush(a, i);
        assert(a[3] == 3);
        arrins(a, 3, -1);
        assert(a[3] == -1);
        arrins(a, 6, -2);
        assert(a[6] == -2);
        int n = arrlen(a);
        for (int i = n; i < n + 5; i++) {
            arrins(a, i, i);
            assert(a[i] == i);
        }
        arrfree(a);
    }
    {
        a = arrnew;
        for (int i = 0; i < 1000; i++)
            arrpush(a, i);
        assert(arrlen(a) == 1000);
        arrsetlen(a, 500);
        assert(arrlen(a) == 500);
        arrpush(a, -1);
        assert(arrlen(a) == 501);
        assert(a[500] == -1);
        arrfree(a);
    }
    {
        struct sa {
            int a, b;
        };
        struct sa ee, temp, *sa;
        a = arrnew;
        assert(sizeof(*sa) == sizeof(int) * 2);
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
        sa = arrnew;
        temp.a = 1;
        temp.b = 5;
        ee.a = 1;
        temp.b = 5;
        arrpush(sa, temp);
        arrpush(sa, ee);
        assert(arrpop(sa).a == 1);
        assert(arrpop(sa).b == 5);
        arrfree(a);
        arrfree(sa);
    }
}
