/* sjson.h - json parser 
 * sleepntsheep 2022 
 *
 * Usage - 
 *     sjsontokarr *toks = sjson_lex(strdup("{\"example\": \"json string\"}"));
 *     ** sjson_lex does modify the string,
 *     so you must use strdup on string literal
 *
 *     sjson *json = sjson_parse(toks);
 *
 *  sjson type store everything, 
 *

    typedef struct sjson {
        struct sjson *next;
        struct sjson *prev;
        int type;
        struct sjson *childvalue;
        struct sjson *tail;
        double numbervalue;
        char *stringvalue;
        char *key;
        enum sjson_err err;
    } sjson;

 * next and prev is used for doubly linked list in object and array
 * no hashmap is used here, doubly linked list is more simple, despite being O(n) lookup
 * childvalue store the doubly linked list of all child, if type is object or array
 * tail store last child, this is used for sjson_addchild function to be O(1)
 * stringvalue store string if type is string
 * numbervalue store double number if type is number
 *
 * To see/check content of json do sjson_debug_print(json)
 */

#ifndef SHEEP_SJSON_H
#define SHEEP_SJSON_H
#include <stddef.h>

static int sjson_isdigit(char c) {
    return ('0' <= c && c <= '9') || c == '-' || c == '+' || c == '.';
}

static int sjson_iswhitespace(char c) {
    return c == ' ' || c == '\n' || c == '\t';
}

#define SJSON_TYPES_LIST \
    SJSON_X(SJSON_INVALID),\
    SJSON_X(SJSON_NULL),   \
    SJSON_X(SJSON_FALSE),  \
    SJSON_X(SJSON_TRUE),   \
    SJSON_X(SJSON_NUMBER), \
    SJSON_X(SJSON_STRING), \
    SJSON_X(SJSON_ARRAY),  \
    SJSON_X(SJSON_OBJECT),

#define SJSON_ERRORS_LIST \
    SJSON_X(SJSON_SUCCESS), \
    SJSON_X(SJSON_NO_TERMINATING_SQUAREBRACKET), \
    SJSON_X(SJSON_NO_TERMINATING_DOUBLEQUOTE), \
    SJSON_X(SJSON_NO_TERMINATING_BRACE), \
    SJSON_X(SJSON_NULL_REFERENCE), \
    SJSON_X(SJSON_INVALID_TYPE), \
    SJSON_X(SJSON_OBJECT_NO_MATCHING_KEY), \
    SJSON_X(SJSON_OUT_OF_BOUND), \
    SJSON_X(SJSON_UNKNOWN_CHARACTER), \
    SJSON_X(SJSON_TRAILING_COMMA), \
    SJSON_X(SJSON_MISSING_COLON), 

#define SJSON_TOKENS_LIST \
    SJSON_X(SJSON_TKLBRACE), \
    SJSON_X(SJSON_TKRBRACE), \
    SJSON_X(SJSON_TKSTRINGLITERAL), \
    SJSON_X(SJSON_TKNUMBERLITERAL), \
    SJSON_X(SJSON_TKNULL), \
    SJSON_X(SJSON_TKFALSE), \
    SJSON_X(SJSON_TKTRUE), \
    SJSON_X(SJSON_TKLSQUAREBRACKET), \
    SJSON_X(SJSON_TKRSQUAREBRACKET), \
    SJSON_X(SJSON_TKCOMMA), \
    SJSON_X(SJSON_TKCOLON),


#define SJSON_X(a) a
enum sjson_type { SJSON_TYPES_LIST };
enum sjson_err { SJSON_ERRORS_LIST };
enum sjson_tokens { SJSON_TOKENS_LIST };
#undef SJSON_X
#define SJSON_X(a) #a
static const char *sjson_typenames[] = { SJSON_TYPES_LIST };
static const char *sjson_errnames[] = { SJSON_ERRORS_LIST };
static const char *sjson_tokennames[] = { SJSON_TOKENS_LIST };
#undef SJSON_X

typedef struct sjson {
    struct sjson *next;
    struct sjson *prev;
    int type;
    struct sjson *childvalue; /* array or object */
    struct sjson *tail;
    double numbervalue;
    char *stringvalue;
    char *key;
    enum sjson_err err;
} sjson;

typedef struct sjsontok {
    int type;
    char *start;
    char *end;
} sjsontok;

typedef struct sjsontokarr {
    size_t length, capacity;
    size_t cur;
    sjsontok *a;
} sjsontokarr;

#define sjson_foreach(json, iter) \
    for (sjson *iter = (json)->childvalue; iter != NULL; iter = (iter)->next) 

sjson *sjson_parse(sjsontokarr *toks);
sjsontokarr *sjson_lex(char *s);
sjson *sjson_array_get(sjson *json, size_t i);
int sjson_array_set(sjson *json, char *key, sjson *value);
int sjson_array_del(sjson *json, char *key, sjson *value);
sjson *sjson_object_get(sjson *json, char *key);
int sjson_object_del(sjson *json, char *key);
int sjson_object_set(sjson *json, char *key, sjson *value);
int sjson_deletechild(sjson *json, sjson *child);
int sjson_addchild(sjson *json, sjson *child);

int sjson_object_set(sjson *json, char *key, sjson *value);
#define sjson_array_push sjson_addchild

void sjson_debug_print(sjson *j, int indent);
void sjson_register_logger(int (*logger)(const char*, ...));

#ifndef SJSON_MALLOC
#define SJSON_MALLOC malloc
#endif /* SJSON_MALLOC */
#ifndef SJSON_CALLOC
#define SJSON_CALLOC calloc
#endif /* SJSON_CALLOC */
#ifndef SJSON_REALLOC
#define SJSON_REALLOC realloc 
#endif /* SJSON_REALLOC */


#endif /* SHEEP_SJSON_H */

#ifdef SHEEP_SJSON_IMPLEMENTATION

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static char getescape(char c) {
    switch (c) {
        case '\\':
            return '\\';
        case 'n':
            return '\n';
        case 't':
            return '\t';
        case '"':
            return '"';
        case 'r':
            return '\r';
        case 'b':
            return '\b';
        default:
            return -1;
    }
}

#define SJSON_LOG(errnum) \
    logger("%s\n", sjson_errnames[errnum])

int sjson_logger_dummy(const char *fmt, ...);
static int (*logger)(const char*, ...) = sjson_logger_dummy;

int sjson_logger_dummy(const char *fmt, ...) {return 0;}

void sjson_register_logger(int (*newlogger)(const char*, ...)) {
    logger = newlogger;
}

sjson *sjson_new(int type) {
    sjson *sj = SJSON_CALLOC(1, sizeof(sjson));
    sj->type = type;
    return sj;
}

sjsontok sjsontok_new(int type, char *start, char *end) {
    return (sjsontok) {
        .type = type,
        .start = start,
        .end = end,
    };
}


sjsontokarr *sjsontokarr_new() {
    sjsontokarr *arr = malloc(sizeof(sjsontokarr));
    arr->length = 0,
    arr->capacity = 4,
    arr->cur = 0;
    arr->a = malloc(sizeof (sjsontok) * arr->capacity);
    return arr;
}

void sjsontokarr_push(sjsontokarr *arr, sjsontok tok) {
    if (arr->length == arr->capacity)
        arr->a = SJSON_REALLOC(arr->a, sizeof (sjsontok) * (arr->capacity *= 2));
    arr->a[arr->length++] = tok;
}

sjsontokarr *sjson_lex(char *s) {
    sjsontokarr *arr = sjsontokarr_new();
    while (*s) {
        switch (*s) {
            case '[':
                sjsontokarr_push(arr, sjsontok_new(SJSON_TKLSQUAREBRACKET, s, s));
                break;
            case ']': 
                sjsontokarr_push(arr, sjsontok_new(SJSON_TKRSQUAREBRACKET, s, s));
                break;
            case '{': 
                sjsontokarr_push(arr, sjsontok_new(SJSON_TKLBRACE, s, s));
                break;
            case '}': 
                sjsontokarr_push(arr, sjsontok_new(SJSON_TKRBRACE, s, s));
                break;
            case ',':
                sjsontokarr_push(arr, sjsontok_new(SJSON_TKCOMMA, s, s));
                break;
            case ':':
                sjsontokarr_push(arr, sjsontok_new(SJSON_TKCOLON, s, s));
                break;
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
            case '+':
            case '-':
            {
                char *start = s;
                while (sjson_isdigit(*s))
                    s++;
                s--;
                sjsontokarr_push(arr, sjsontok_new(SJSON_TKNUMBERLITERAL, start, s));
                break;
            }
            case '"':
            {
                char *start = ++s;
                while (*s != '"') {
                    if (*s == '\\') {
                        s++;
                    }
                    s++;
                }
                *s = 0;
                sjsontokarr_push(arr, sjsontok_new(SJSON_TKSTRINGLITERAL, start, s-1));
                break;
            }
            case 'n':
                sjsontokarr_push(arr, sjsontok_new(SJSON_TKNULL, s, s+3));
                s += 3;
                break;
            case 't':
                sjsontokarr_push(arr, sjsontok_new(SJSON_TKTRUE, s, s+3));
                s += 3;
                break;
            case 'f':
                sjsontokarr_push(arr, sjsontok_new(SJSON_TKFALSE, s, s+4));
                s += 4;
                break;
            case ' ':
            case '\t':
            case '\n':
                break;
            default:
                logger("unknown char : %c", *s);
                break;
        }
        s++;
    }
    return arr;
}

sjson *sjson_parseobject(sjsontokarr *toks) {
    sjson *obj = sjson_new(SJSON_OBJECT);
    while (toks->a[toks->cur].type != SJSON_TKRBRACE) {
        char *key = toks->a[toks->cur].start;
        toks->cur++;
        if (toks->a[toks->cur].type != SJSON_TKCOLON)
            SJSON_LOG(SJSON_MISSING_COLON);
        toks->cur++;
        sjson *child = sjson_parse(toks);
        child->key = key;
        if (toks->a[toks->cur].type == SJSON_TKCOMMA)
            toks->cur++;
        sjson_addchild(obj, child);
    }
    toks->cur++;
    return obj;
}

sjson *sjson_parsearray(sjsontokarr *toks) {
    sjson *obj = sjson_new(SJSON_ARRAY);
    while (toks->a[toks->cur].type != SJSON_TKRSQUAREBRACKET) {
        sjson *child = sjson_parse(toks);
        sjson_addchild(obj, child);
        if (toks->a[toks->cur].type == SJSON_TKCOMMA)
            toks->cur++;
    }
    toks->cur++;
    return obj;
}

sjson *sjson_parse(sjsontokarr *toks) {
    if (toks->cur >= toks->length)
        return sjson_new(SJSON_INVALID);
    sjsontok tok = toks->a[toks->cur];
    toks->cur++;
    switch (tok.type) {
        case SJSON_TKTRUE:
            return sjson_new(SJSON_TRUE);
        case SJSON_TKFALSE:
            return sjson_new(SJSON_FALSE);
        case SJSON_TKNULL:
            return sjson_new(SJSON_NULL);
        case SJSON_TKLBRACE:
            return sjson_parseobject(toks);
        case SJSON_TKLSQUAREBRACKET:
            return sjson_parsearray(toks);
        case SJSON_TKNUMBERLITERAL:
            {
                sjson *j = sjson_new(SJSON_NUMBER);
                char *end = tok.end;
                char temp = end[1];
                end[1] = 0;
                j->numbervalue = strtod(tok.start, NULL);
                end[1] = temp;
                return j;
            }
        case SJSON_TKSTRINGLITERAL:
            {
                sjson *j = sjson_new(SJSON_STRING);
                j->stringvalue = tok.start;
                return j;
            }
        default:
            logger("Unknown token: %s %s\n", sjson_tokennames[tok.type], tok.start);
            return NULL;
            ;
    }
}

int sjson_deletechild(sjson *json, sjson *child) {
    if (json->tail == child) {
        if (json->childvalue == child)
            json->tail = json->childvalue = NULL;
        else
            json->tail = child->prev;
    } else if (json->childvalue == child) {
        json->childvalue == child->next;
    }
    if (child->prev)
        child->prev->next = child->next;
    if (child->next)
        child->next->prev = child->prev;
}

sjson *sjson_object_get(sjson *json, char *key) {
    sjson_foreach(json, iter)
        if (!strcmp(iter->key, key))
            return iter;
    return NULL;
}

int sjson_object_del(sjson *json, char *key) {
    sjson_foreach(json, iter) {
        if (!strcmp(iter->key, key)) {
            sjson_deletechild(json, iter);
            logger("Deleted key : %s\n", key);
        }
    }
    return 0;
}

int sjson_object_set(sjson *json, char *key, sjson *value) {
    sjson_object_del(json, key);
    value->key = key;
    sjson_addchild(json, value);
    return 0;
}

int sjson_addchild(sjson *json, sjson *child) {
    if (json->tail == NULL || json->childvalue == NULL) {
        json->tail = json->childvalue = child;
    } else {
        json->tail->next = child;
        child->prev = json->tail;
        json->tail = child;
    }
    return 0;
}

sjson *sjson_array_get(sjson *json, size_t i) {
    size_t x = 0;
    sjson_foreach(json, iter)
        if (x++ == i)
            return iter;
    return NULL;
}

void sjson_print_tokarr(sjsontokarr *arr) {
    for (int i =0 ; i< arr->length; i++)
     {
         printf("%s %.*s\n", sjson_tokennames[arr->a[i].type], arr->a[i].end - arr->a[i].start + 1,  arr->a[i].start);
     }
}

void sjson_debug_print(sjson *j, int indent) {
    if (j == NULL) {
        SJSON_LOG(SJSON_NULL_REFERENCE);
        return;
    }
    for (int i = 0; i < indent; i++)
        printf(" ");
    printf("JSON :   type = %d   name = %s   num = %5.6lf   str = %s\n",
            j->type, j->key, j->numbervalue, j->stringvalue);
    if (j->childvalue) sjson_debug_print(j->childvalue, indent+1);
    if (j->next) sjson_debug_print(j->next, indent);
}

int sjson_deserialize(FILE *fp, sjson *json) {
    switch (json->type) {
        case SJSON_INVALID:
            break;
        case SJSON_NUMBER:
            fprintf(fp, "%lf", json->numbervalue);
            break;
        case SJSON_STRING:
            fprintf(fp, "\"%s\"", json->stringvalue);
            break;
        case SJSON_NULL:
            fprintf(fp, "null");
            break;
        case SJSON_TRUE:
            fprintf(fp, "true");
            break;
        case SJSON_FALSE:
            fprintf(fp, "false");
            break;
        case SJSON_OBJECT:
            fprintf(fp, "{");
            sjson_foreach(json, it) {
                fprintf(fp, "\"%s\": ", it->key);
                sjson_deserialize(fp, it);
                if (it->next != NULL)
                    fprintf(fp, ",");
            }
            fprintf(fp, "}");
            break;
        case SJSON_ARRAY:
            fprintf(fp, "[");
            sjson_foreach(json, it) {
                sjson_deserialize(fp, it);
                if (it->next != NULL)
                    fprintf(fp, ",");
            }
            fprintf(fp, "]");
            break;
        default:
            SJSON_LOG(SJSON_INVALID_TYPE);
            break;
    }
}


#endif /* SHEEP_SJSON_IMPLEMENTATION */
