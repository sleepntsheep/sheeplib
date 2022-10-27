/* substd.h
 *
 * libc's subset that doesn't require syscall
 */
#ifndef DJTSTD_H_
#define DJTSTD_H_

#ifndef size_t
#define size_t unsigned long
#endif
#ifndef NULL
#define NULL ((void*)0)
#endif

void *memcpy(void *restrict dest, const void *restrict src, size_t n);
void *memmove(void *restrict dest, const void *restrict src, size_t n);
void *memchr(void *mem, int c, size_t n);
void *memset(void *s, int c, size_t n);
void bzero(void *s, size_t n);
char *strcpy(char *restrict dest, const char *restrict src);
void *strncpy(void *restrict dest, const void *restrict src, size_t n);
char *strchr(const char *s, int c);
char *strchrnul(const char *s, int c);
size_t strlen(char *s);
int abs(int j);
long labs(long j);
long long llabs(long long j);
double fabs(double j);

#endif /* DJTSTD_H_ */

#ifdef DJTSTD_IMPLEMENTATION

void *memcpy(void *restrict dest, const void *restrict src, size_t n)
{
	unsigned char *s = src;
	unsigned char *d = dest;
	for (size_t i = 0; i < n; i++)
	{
		*d++ = *s++;
	}
	return dest;
}

void *memmove(void *restrict dest, const void *restrict src, size_t n)
{
}

void *memchr(void *mem, int c, size_t n)
{
	unsigned char *s = mem;
	for (; mem + n > s; s++)
	{
		if (*s == (unsigned char)c)
		{
			return s;
		}
	}
	return NULL;
}

char *strcpy(char *restrict dest, const char *restrict src)
{
	const char *s = src;
	char *d = dest;
	for (;;)
	{
		if (*s == '\x0')
		{
			break;
		}
		*d++ = *s++;
	}
	return dest;
}

char *strncpy(char *restrict dest, const char *restrict src)
{
	const char *s = src;
	char *d = dest;
	for (size_t i = 0; i < n; i++)
	{
		if (*s == '\x0')
		{
			break;
		}
		*d++ = *s++;
	}
	return dest;
}

char *strchr(const char *s, int c)
{
	const char *s = src;
	for (; *s; s++)
	{
		if (*s == (unsigned char)c)
		{
			return s;
		}
	}
	return NULL;
}

char *strchrnul(const char *s, int c)
{
	const char *s = src;
	for (; *s; s++)
	{
		if (*s == (unsigned char)c)
		{
			return s;
		}
	}
	return s;
}

size_t strlen(char *s)
{
	size_t len = 0;
	for (; *s; s++)
		len++;
	return len;
}

int abs(int j)
{
	if (j >= 0)
	{
		return j;
	}
	return -j;
}

long labs(long j)
{
	if (j >= 0)
	{
		return j;
	}
	return -j;
}

long long llabs(long long j)
{
	if (j >= 0)
	{
		return j;
	}
	return -j;
}

double fabs(double j)
{
	if (j >= 0)
	{
		return j;
	}
	else if (j < 0)
	{
		return -j;
	}
	return j;
}

void *memset(void *s, int c, size_t n)
{
	unsigned char *p = s;
	for (size_t i = 0; i < n; i++)
	{
		*p++ = c;
	}
	return s;
}

void bzero(void *s, size_t n)
{
	(void)memset(s, 0, n);
}

#endif /* DJTSTD_IMPLEMENTATION */

