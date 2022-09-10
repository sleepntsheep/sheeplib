#ifndef SHEEP_SJSON_H
#define SHEEP_SJSON_H

static int sjson_isdigit(char c) {
    return '0' <= c && c <= '9';
}

static int sjson_iswhitespace(char c) {
    return c == ' ' || c == '\n' || c == '\t';
}

enum sjson_type {
    SJSON_INVALID,
    SJSON_NULL,
    SJSON_FALSE,
    SJSON_TRUE,
    SJSON_NUMBER,
    SJSON_STRING,
    SJSON_ARRAY,
    SJSON_OBJECT,
};

typedef struct sjson {
    struct sjson *next;
    struct sjson *prev;
    int type;

    struct sjson *childvalue; /* array or object */
    double numbervalue;
    char *stringvalue;
    char *key;
} sjson;

typedef struct sjsonwithstr {
    sjson *sjson;
    char *s;
} sjsonwiths;

sjsonwiths sjson_parse(char *s);

void sjson_debug_print(sjson *j, int indent);

#endif /* SHEEP_SJSON_H */

#ifdef SHEEP_SJSON_IMPLEMENTATION

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define SHEEP_CALLOC calloc

sjson *sjson_new_type(int type) {
    sjson *sj = SHEEP_CALLOC(1, sizeof(sjson));
    sj->type = type;
    return sj;
}

sjsonwiths sjson_parseobject(char *s) {
#ifdef SHEEP_SJSON_DEBUG
    printf("sjson_parseobject parsing %s\n", s);
    fflush(stdout);
#endif
    sjson *obj = sjson_new_type(SJSON_OBJECT);
    sjson *tail = obj->childvalue;
    for (; *s != '}';) {
        char *key;
        sjsonwiths parsed;
        while (sjson_iswhitespace(*s)) s++;

        /* parse key */
        while (*s++ != '"');
        key = s;
        while (*s != '"') s++;
        *s++ = 0;
        while (*s == ' ' || *s == ':') s++;

        /* parse value */
        parsed = sjson_parse(s);
        s = parsed.s;
        parsed.sjson->key = key;
        if (tail == NULL) {
            obj->childvalue = parsed.sjson;
            tail = obj->childvalue;
        }
        else {
            tail->next = parsed.sjson;
            tail = tail->next;
        }

        while (*s == ',' || sjson_iswhitespace(*s)) s++;
    }
    return (sjsonwiths) {
        .sjson = obj,
        .s = s,
    };
}

sjsonwiths sjson_parsestring(char *s) {
    sjson *sj = sjson_new_type(SJSON_STRING);
    sj->stringvalue = s;
    while (*s != '\"') s++;
    *s = 0;
    s++;
    return (sjsonwiths) {
        .sjson = sj,
        .s = s,
    };
}

sjsonwiths sjson_parsenumber(char *s) {
    sjson *sj = sjson_new_type(SJSON_NUMBER);
    double fact = 1;
    int point = 0;
    sj->numbervalue = 0;
    if (*s == '-') {
        s++;
        fact = -1;
    }
    for (; sjson_isdigit(*s) || *s == '.'; s++) {
        if (*s == '.') {
            point = 1;
            continue;
        }
        sj->numbervalue *= 10;
        sj->numbervalue += *s - '0';
        if (point) {
            fact /= 10L;
        }
    }
    sj->numbervalue *= fact;
    return (sjsonwiths) {
        .sjson = sj,
        .s = s,
    };
}

sjsonwiths sjson_parsearray(char *s) {
    sjson *arr = sjson_new_type(SJSON_ARRAY);
    sjson *tail = arr->childvalue;
    for (; *s != ']'; s++) {
        sjsonwiths parsed;
        switch (*s) {
            case ',':
            case ' ':
                continue;
            default:
                parsed = sjson_parse(s);
                s = parsed.s;
                if (tail == NULL) {
                    arr->childvalue = parsed.sjson;
                    tail = arr->childvalue;
                } else {
                    tail->next = parsed.sjson;
                    tail = tail->next;
                }
                break;
        }
    }
    return (sjsonwiths) {
        .sjson = arr,
        .s = s,
    };
}

sjsonwiths sjson_parse(char *s) {
    while (sjson_iswhitespace(*s)) s++;
    switch (*s) {
        case '{':
            return sjson_parseobject(s+1);
        case '"':
            return sjson_parsestring(s+1);
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return sjson_parsenumber(s);
        case '[':
            return sjson_parsearray(s+1);
        case 'n':
            return (sjsonwiths) {
                .sjson = sjson_new_type(SJSON_NULL),
                .s = s + 4,
            };
        case 'f':
            return (sjsonwiths) {
                .sjson = sjson_new_type(SJSON_FALSE),
                .s = s + 5,
            };
        case 't':
            return (sjsonwiths) {
                .sjson = sjson_new_type(SJSON_TRUE),
                .s = s + 4,
            };
        default:
            ;
    }
}

void sjson_debug_print(sjson *j, int indent) {
    for (int i = 0; i < indent; i++)
        printf(" ");
    printf("JSON :   type = %d   name = %s   num = %5.2lf   str = %s\n",
            j->type, j->key, j->numbervalue, j->stringvalue);
    if (j->childvalue) sjson_debug_print(j->childvalue, indent+1);
    if (j->next) sjson_debug_print(j->next, indent);
}

#endif /* SHEEP_SJSON_IMPLEMENTATION */
