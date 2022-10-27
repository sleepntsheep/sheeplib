#ifndef FABE64_H_
#define FABE64_H_
#include <stddef.h>
#include <stdint.h>

int fabe64_encode(const unsigned char *, size_t , char *, size_t );
int fabe64_decode(const char *, unsigned char *, size_t);

#endif /* FABE64_H_ */

#ifdef FABE64_IMPLEMENTATION

int fabe64_encode(const unsigned char *in, size_t inn, char *out, size_t outn)
{
    static const char _fabe64_encode_lookup[] = 
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    const unsigned char *const is = in;
    const unsigned char *const ie = in + inn;
    char *const os = out;
    char *const oe = out + outn;

    while (in < ie)
    {
        uint32_t tb = in[0] << 16;
        if (in + 1 < ie)
            tb += in[1] << 8;
        if (in + 2 < ie)
            tb += in[2];

        unsigned char n0 = _fabe64_encode_lookup[(tb >> 18) & 63];
        unsigned char n1 = _fabe64_encode_lookup[(tb >> 12) & 63];
        unsigned char n2 = _fabe64_encode_lookup[(tb >>  6) & 63];
        unsigned char n3 = _fabe64_encode_lookup[(tb      ) & 63];

        if (out >= oe) goto out2small;
        *out++ = n0;
        if (out >= oe) goto out2small;
        *out++ = n1;
        in++;

        if (in < ie)
        {
            if (out >= oe) goto out2small;
            *out++ = n2;
            in++;
        }

        if (in < ie)
        {
            if (out >= oe) goto out2small;
            *out++ = n3;
            in++;
        }
    }

    for (int pad = (out - os) & 3;
            pad != 0 && pad < 4; pad++)
    {
        *out++ = '=';
    }

    return out - os;
out2small:
    return -1;
}

int fabe64_decode(const char *in, unsigned char *out, size_t outn)
{
    /* TODO: make this actually work */
    static const unsigned char _fabe64_decode_lookup[] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, };

    unsigned char *const oe = out + outn;
    unsigned char *const os = out;

    while (*in)
    {
        for (int i = 0; i < 4; i++)
        {
            if (*in == '=') goto done;
            if (out >= oe) goto out2small;
            *out++ = _fabe64_decode_lookup[*in++];
        }
    }
done:

    return out - os;
out2small:
    return -1;
}

#endif /* FABE64_IMPLEMENTATION */

