/**
 * @file sjson.h
 * @brief Simple json parser
 *
 * Usage: take a look at `struct sjson`, it represents a json value
 * Notes: object and array children are stored as linked list,
 *        this is for simplicity and predictable memory usage
 *        if you are going to do a lot of children lookup,
 *        you should iterate over them yourself or use different library
 *
 * Example:
 *        look at test/test.c, describe(json)
 *
 * v0.0.3 - sleepntsheep 2022
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SHEEP_SJSON_H
#define SHEEP_SJSON_H
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#define SJSON_TYPES_LIST                                                       \
    SJSON_X(SJSON_INVALID), SJSON_X(SJSON_NULL), SJSON_X(SJSON_FALSE),         \
        SJSON_X(SJSON_TRUE), SJSON_X(SJSON_NUMBER), SJSON_X(SJSON_STRING),     \
        SJSON_X(SJSON_ARRAY), SJSON_X(SJSON_OBJECT),

#define SJSON_TOKENS_LIST                                                      \
    SJSON_X(SJSON_TKINVALID), SJSON_X(SJSON_TKLBRACE),                         \
        SJSON_X(SJSON_TKRBRACE), SJSON_X(SJSON_TKSTRINGLITERAL),               \
        SJSON_X(SJSON_TKNUMBERLITERAL), SJSON_X(SJSON_TKNULL),                 \
        SJSON_X(SJSON_TKFALSE), SJSON_X(SJSON_TKTRUE),                         \
        SJSON_X(SJSON_TKLSQUAREBRACKET), SJSON_X(SJSON_TKRSQUAREBRACKET),      \
        SJSON_X(SJSON_TKCOMMA), SJSON_X(SJSON_TKCOLON),

#define SJSON_ERR_LIST                                                         \
    SJSON_X(SJSON_SUCCESS), SJSON_X(SJSON_ERR_TRAILING_COMMA),                 \
        SJSON_X(SJSON_ERR_NO_TERMINATING_BRACE),                               \
        SJSON_X(SJSON_ERR_NO_TERMINATING_BRACKET),                             \
        SJSON_X(SJSON_ERR_NO_MATCHING_MEMBER), SJSON_X(SJSON_ERR_NO_MEMORY),   \
        SJSON_X(SJSON_ERR_WRONG_TYPE), SJSON_X(SJSON_ERR_UNKNOWN_TOKEN),       \
        SJSON_X(SJSON_ERR_NO_TERMINATING_DOUBLE_QUOTE),                        \
        SJSON_X(SJSON_ERR_INVALID_SOURCE),                                     \
        SJSON_X(SJSON_ERR_INVALID_ESCAPE_SEQUENCE),                            \
        SJSON_X(SJSON_ERR_NULL_REFERENCE),

#define SJSON_X(a) a
enum sjson_type { SJSON_TYPES_LIST };
enum sjson_tokens { SJSON_TOKENS_LIST };
enum SjsonResult { SJSON_ERR_LIST };
#undef SJSON_X
#define SJSON_X(a) #a
static const char *sjson_type_names[] = {SJSON_TYPES_LIST};
static const char *sjson_err_names[] = {SJSON_ERR_LIST};
static const char *sjson_token_names[] = {SJSON_TOKENS_LIST};
#undef SJSON_X

typedef enum SjsonResult sjson_resultnum;

/**
 * @brief char buffer
 */
typedef struct sjsonbuf {
    size_t len; /** length */
    size_t cap; /** private: capacity */
    char *buf;  /** buffer */
} sjsonbuf;

struct sjson_value {
    double num;          /** number value */
    const char *str;     /** string value */
    struct sjson *child; /** first node in children linked list */
    struct sjson *tail;  /** last node in children linked list */
};

/**
 * @brief struct for representing json
 */
typedef struct sjson {
    int type;             /** type of json object */
    struct sjson_value v; /** value of object */
    struct sjson *next;   /** next sibling object */
    struct sjson *prev;   /** previous sibling object */
    const char *key;      /** key of object, if any */
} sjson;

typedef struct {
    sjson *json; /** json object */
    int err;     /** SJSON_SUCCESS (0) if no err, else non-zero */
} sjson_result;

typedef struct sjsontok {
    int type;
    const char *start;
    const char *end;
} sjsontok;

typedef struct sjsontokarr {
    size_t length, capacity;
    size_t cur;
    sjsontok *a;
} sjsontokarr;

typedef struct sjsonlexer {
    const char *start, *end, *c;
    sjsontokarr toks;
} sjsonlexer;

typedef struct sjsonparser {
    size_t i;
    sjsontokarr toks;
} sjsonparser;

/**
 * @brief iterate through all child of sjson object
 * @param json parent json to iterate over
 * @param iter name of iterator
 */
#define sjson_foreach(json, iter)                                              \
    for (sjson *iter = (json)->v.child; (iter) != NULL; iter = (iter)->next)

/**
 * @brief create new sjson object with type
 */
sjson_result sjson_new(int type);

/**
 * @brief recursively free all children of json
 * and then free json
 */
void sjson_free(sjson *json);

/**
 * @brief deserialize C-string to sjson *
 * @param s C-string
 * @param len length of parameter s
 */
sjson_result sjson_deserialize(const char *s, size_t len);

/**
 * @brief serialize sjson * to C-string in sjsonbuf
 * @return buffer holding length, capacity, and pointer to C-string
 */
sjsonbuf sjson_serialize(sjson *json);

static void sjsonlexer_init(sjsonlexer *lexer, const char *s, size_t len);
static SjsonResult sjsonlexer_lex(sjsonlexer *lexer);
static sjson_result sjson_parse(sjsontokarr *toks);

/**
 * @brief get child by index
 */
sjson_result sjson_array_get(sjson *json, size_t i);
/**
 * @brief replace index-th child of array to json
 * @param json array
 * @param i index of child to replace
 * @param x new child value
 */
SjsonResult sjson_array_set(sjson *json, size_t i, sjson *x);
/**
 * @brief remove i-th child of array
 * @param json array
 * @param i index of child element to remove
 */
SjsonResult sjson_array_delete(sjson *json, size_t i);
/**
 * @brief get first child of object with matching key
 * @param json object
 * @param key key to get
 */

sjson_result sjson_object_get(sjson *json, char *key);
/**
 * @brief set key property of object to json
 * @param json object
 * @param key key of new child
 * @param value new child to add
 *
 * This function call sjson_object_delete_all(json, key)
 * before adding child. Causing every prior child having the same key
 * to be deleted
 */
SjsonResult sjson_object_set(sjson *json, char *key, sjson *value);
/**
 * @brief delete all child with matching key
 * @param json parent node
 * @param key key to delete
 */
SjsonResult sjson_object_delete_all(sjson *json, char *key);
// SjsonResult sjson_array_del(sjson *json, char *key, sjson *value);

/**
 * @brief add child to json's children
 * @param json parent node
 * @param child child node
 */
SjsonResult sjson_addchild(sjson *json, sjson *child);

/**
 * @brief delete child from json's children
 * @param json parent node
 * @param child child node
 * @return SJSON_SUCCESS (0) if success, else non-zero
 */
SjsonResult sjson_deletechild(sjson *json, sjson *child);

/**
 * @brief add element to end of array
 * @param json parent node
 * @param child child node
 * @return SJSON_SUCCESS (0) if success, else non-zero
 */
#define sjson_array_push sjson_addchild

#endif /* SHEEP_SJSON_H */

#ifdef SHEEP_SJSON_IMPLEMENTATION

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static sjsontokarr sjsontokarr_new() {
    return (sjsontokarr){
        .length = 0,
        .capacity = 4,
        .cur = 0,
        .a = (sjsontok *)malloc(sizeof(sjsontok) * 4),
    };
}

static void sjsontokarr_push(sjsontokarr *arr, sjsontok tok) {
    if (arr->length == arr->capacity)
        arr->a = (sjsontok *)realloc(arr->a,
                                     sizeof(sjsontok) * (arr->capacity *= 2));
    arr->a[arr->length++] = tok;
}

static sjsontok sjsontokarr_advance(sjsontokarr *arr) {
    if (arr->cur < arr->length - 1)
        return arr->a[arr->cur++];
    return (sjsontok){0};
}

static sjsontok sjsontokarr_peek(sjsontokarr *arr) { return arr->a[arr->cur]; }

static void sjsonlexer_pushtok(sjsonlexer *lexer, int type, const char *start,
                               const char *end) {
    sjsontokarr_push(&lexer->toks, (sjsontok){type, start, end});
}

static void sjsonbuf_init(sjsonbuf *buf) {
    buf->cap = 128;
    buf->len = 0;
    buf->buf = (char *)malloc(buf->cap);
}

static void sjsonbuf_push(sjsonbuf *buf, const void *s, size_t len) {
    while (buf->cap - buf->len <= len + 1)
        buf->cap *= 2;
    buf->buf = (char *)realloc(buf->buf, buf->cap);
    memcpy(buf->buf + buf->len, s, len);
    buf->len += len;
    buf->buf[buf->len] = '\x0';
}

static char sjsonlexer_advance(sjsonlexer *lexer) {
    if (lexer->c <= lexer->end)
        return (lexer->c++)[0];
    return -1;
}

static char sjsonlexer_peek(sjsonlexer *lexer) { return (lexer->c)[0]; }

static bool sjsonlexer_isend(sjsonlexer *lexer) {
    return lexer->c >= lexer->end;
}

static void sjsonlexer_init(sjsonlexer *lexer, const char *s, size_t len) {
    lexer->start = s, lexer->end = s + len, lexer->c = s,
    lexer->toks = sjsontokarr_new();
}

static SjsonResult sjsonlexer_lexnumber(sjsonlexer *lexer) {
    const char *numberstart = lexer->c;
    bool didpoint = false, didsign = false;
    sjsonbuf buf;
    sjsonbuf_init(&buf);

    while (!sjsonlexer_isend(lexer)) {
        char c = sjsonlexer_peek(lexer);
        if (!isdigit(c)) {
            if (c == '+' || c == '-') {
                if (didsign)
                    return SJSON_ERR_INVALID_SOURCE;
                didsign = true;
            } else if (c == '.') {
                if (didpoint)
                    return SJSON_ERR_INVALID_SOURCE;
                didpoint = true;
            } else {
                break;
            }
        }
        sjsonlexer_advance(lexer);
    }
    lexer->c--;

    /* avoid modification of original source */
    sjsonbuf_push(&buf, numberstart, lexer->c - numberstart + 1);

    sjsonlexer_pushtok(lexer, SJSON_TKNUMBERLITERAL, buf.buf, lexer->c);

    return SJSON_SUCCESS;
}

static SjsonResult sjsonlexer_lexstring(sjsonlexer *lexer) {
    if (sjsonlexer_advance(lexer) != '\"')
        return SJSON_ERR_WRONG_TYPE;

    const char *stringend = lexer->c;

    /* find terminating double quote, need to escape \" */
    while (stringend[0] != '\"') {
        if (stringend > lexer->end)
            return SJSON_ERR_NO_TERMINATING_DOUBLE_QUOTE;
        if (stringend[0] == '\\')
            stringend++;
        stringend++;
    }

    /* can't just push new token, escape character reasons */
    sjsonbuf buf;
    sjsonbuf_init(&buf);
    while (lexer->c < stringend) {
        char ch = sjsonlexer_advance(lexer);
        if (ch == '\\') {
            int advance_ret;
            if ((advance_ret = sjsonlexer_advance(lexer)) == -1)
                return SJSON_ERR_NO_TERMINATING_DOUBLE_QUOTE;
            /* deal with escape character */
            switch (advance_ret) {
            case '\\':
                sjsonbuf_push(&buf, "\\", 1);
                break;
            case 'n':
                sjsonbuf_push(&buf, "\n", 1);
                break;
            case 'f':
                sjsonbuf_push(&buf, "\f", 1);
                break;
            case 'r':
                sjsonbuf_push(&buf, "\r", 1);
                break;
            case 't':
                sjsonbuf_push(&buf, "\t", 1);
                break;
            case '\"':
                sjsonbuf_push(&buf, "\"", 1);
                break;
            case '/':
                sjsonbuf_push(&buf, "/", 1);
                break;
                /*
            case 'u': {
                break;
            }
            */
            default:
                break;
            }
        } else {
            sjsonbuf_push(&buf, &ch, 1);
        }
    }
    sjsonlexer_pushtok(lexer, SJSON_TKSTRINGLITERAL, buf.buf,
                       buf.buf + buf.len);
    return SJSON_SUCCESS;
}

SjsonResult sjsonlexer_lexprimitive(sjsonlexer *lexer) {
    int type;
    ptrdiff_t len;
    switch (sjsonlexer_peek(lexer)) {
    case 't':
        type = SJSON_TKTRUE;
        len = 4;
        break;
    case 'f':
        type = SJSON_TKFALSE;
        len = 5;
        break;
    case 'n':
        type = SJSON_TKNULL;
        len = 4;
        break;
    default:
        return SJSON_ERR_WRONG_TYPE;
    }
    if (lexer->end - lexer->start + 1 < len) {
        return SJSON_ERR_INVALID_SOURCE;
    }
    sjsonlexer_pushtok(lexer, type, lexer->c, lexer->c + len - 1);
    lexer->c += len - 1;

    return SJSON_SUCCESS;
}

SjsonResult sjsonlexer_lex(sjsonlexer *lexer) {
    while (!sjsonlexer_isend(lexer)) {
        SjsonResult ret;
        switch (sjsonlexer_peek(lexer)) {
        case '[':
            sjsonlexer_pushtok(lexer, SJSON_TKLSQUAREBRACKET, lexer->c,
                               lexer->c);
            break;
        case ']':
            sjsonlexer_pushtok(lexer, SJSON_TKRSQUAREBRACKET, lexer->c,
                               lexer->c);
            break;
        case '{':
            sjsonlexer_pushtok(lexer, SJSON_TKLBRACE, lexer->c, lexer->c);
            break;
        case '}':
            sjsonlexer_pushtok(lexer, SJSON_TKRBRACE, lexer->c, lexer->c);
            break;
        case ',':
            sjsonlexer_pushtok(lexer, SJSON_TKCOMMA, lexer->c, lexer->c);
            break;
        case ':':
            sjsonlexer_pushtok(lexer, SJSON_TKCOLON, lexer->c, lexer->c);
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
            if ((ret = sjsonlexer_lexnumber(lexer))) {
                return ret;
            }
            break;
        case '\"':
            if ((ret = sjsonlexer_lexstring(lexer))) {
                return ret;
            }
            break;
        case 'n':
        case 't':
        case 'f':
            if ((ret = sjsonlexer_lexprimitive(lexer))) {
                return ret;
            }
            break;
        case ' ':
        case '\t':
        case '\n':
            break;
        default:
            break;
        }
        sjsonlexer_advance(lexer);
    }
    return SJSON_SUCCESS;
}

sjson_result sjson_new(int type) {
    sjson *json = (sjson *)calloc(1, sizeof(*json));
    if (json == NULL) {
        return (sjson_result){
            .err = SJSON_ERR_NO_MEMORY,
        };
    }
    json->type = type;
    return (sjson_result){.json = json};
}

void sjson_free(sjson *json) {
    if (json->type == SJSON_OBJECT || json->type == SJSON_ARRAY)
        if (json->v.child != NULL)
            sjson_free(json->v.child);
    if (json->next != NULL)
        sjson_free(json->next);
    free(json);
}

static sjson_result sjson_parseobject(sjsontokarr *toks) {
    sjson_result obj = sjson_new(SJSON_OBJECT);
    if (obj.err) {
        return (sjson_result){.err = obj.err};
    }

    if (sjsontokarr_advance(toks).type != SJSON_TKLBRACE) {
        return (sjson_result){.err = SJSON_ERR_INVALID_SOURCE};
    }

    while (sjsontokarr_peek(toks).type != SJSON_TKRBRACE) {
        const char *key = sjsontokarr_advance(toks).start;
        sjson_result child;
        if (sjsontokarr_advance(toks).type != SJSON_TKCOLON) {
            return (sjson_result){.err = SJSON_ERR_NO_TERMINATING_BRACE};
        }
        child = sjson_parse(toks);
        if (child.err) {
            return child;
        }
        child.json->key = key;
        SjsonResult ret = sjson_addchild(obj.json, child.json);
        if (ret != SJSON_SUCCESS) {
            return (sjson_result){.err = ret};
        }
        sjsontok next = sjsontokarr_peek(toks);
        if (next.type == SJSON_TKCOMMA) {
            sjsontokarr_advance(toks);
        } else if (next.type != SJSON_TKRBRACE) {
            return (sjson_result){
                .err = SJSON_ERR_NO_TERMINATING_BRACE,
            };
        }
    }
    return obj;
}

static sjson_result sjson_parsearray(sjsontokarr *toks) {
    sjson_result arr = sjson_new(SJSON_ARRAY);
    if (arr.err)
        return (sjson_result){.err = arr.err};

    if (sjsontokarr_advance(toks).type != SJSON_TKLSQUAREBRACKET)
        return (sjson_result){.err = SJSON_ERR_INVALID_SOURCE};

    while (sjsontokarr_peek(toks).type != SJSON_TKRSQUAREBRACKET) {
        sjson_result child = sjson_parse(toks);
        if (child.err)
            return child;
        else
            sjson_addchild(arr.json, child.json);
        sjsontok next = sjsontokarr_peek(toks);
        if (next.type == SJSON_TKCOMMA)
            sjsontokarr_advance(toks);
        else if (next.type != SJSON_TKRSQUAREBRACKET)
            return (sjson_result){
                .err = SJSON_ERR_NO_TERMINATING_BRACKET,
            };
    }
    return arr;
}

sjson_result sjson_parse(sjsontokarr *toks) {
    sjson_result ret;
    switch (sjsontokarr_peek(toks).type) {
    case SJSON_TKTRUE:
        ret = sjson_new(SJSON_TRUE);
        break;
    case SJSON_TKFALSE:
        ret = sjson_new(SJSON_FALSE);
        break;
    case SJSON_TKNULL:
        ret = sjson_new(SJSON_NULL);
        break;
    case SJSON_TKLBRACE:
        ret = sjson_parseobject(toks);
        if (ret.err)
            return ret;
        break;
    case SJSON_TKLSQUAREBRACKET:
        ret = sjson_parsearray(toks);
        if (ret.err)
            return ret;
        break;
    case SJSON_TKNUMBERLITERAL:
        ret = sjson_new(SJSON_NUMBER);
        if (ret.err)
            return ret;
        /* TODOOOOOO: stop using strtod as it only support base 10 and base 16,
            ideally this should support 0o (octal) and 0b (binary) too */
        ret.json->v.num = strtod(sjsontokarr_peek(toks).start, NULL);
        break;
    case SJSON_TKSTRINGLITERAL:
        ret = sjson_new(SJSON_STRING);
        if (ret.err)
            return ret;
        ret.json->v.str = sjsontokarr_peek(toks).start;
        break;
    default:
        return (sjson_result){
            .err = SJSON_ERR_UNKNOWN_TOKEN,
        };
    }
    sjsontokarr_advance(toks);
    return ret;
}

SjsonResult sjson_replace(sjson *parent, sjson *oldsibling,
                              sjson *newsibling) {
    if (!oldsibling || !newsibling)
        return SJSON_ERR_NULL_REFERENCE;
    if (parent) {
        if (parent->v.tail == oldsibling)
            parent->v.tail == newsibling;
        if (parent->v.child == oldsibling)
            parent->v.child = newsibling;
    }
    if (oldsibling->prev)
        oldsibling->prev->next = newsibling;
    if (oldsibling->next)
        oldsibling->next->prev = newsibling;
    sjson_free(oldsibling);
    return SJSON_SUCCESS;
}

SjsonResult sjson_deletechild(sjson *json, sjson *child) {
    if (json->v.child == NULL)
        return SJSON_ERR_NO_MATCHING_MEMBER;
    if (json->v.tail == child) {
        if (json->v.child == child)
            json->v.tail = json->v.child = NULL;
        else
            json->v.tail = child->prev;
    } else if (json->v.child == child) {
        json->v.child = child->next;
    }
    if (child->prev)
        child->prev->next = child->next;
    if (child->next)
        child->next->prev = child->prev;
    return SJSON_SUCCESS;
}

sjson_result sjson_object_get(sjson *json, char *key) {
    if (json->type != SJSON_OBJECT)
        return (sjson_result){.err = SJSON_ERR_WRONG_TYPE};
    sjson_foreach(json, iter) if (!strcmp(iter->key, key)) return (
        sjson_result){.json = iter};
    return (sjson_result){
        .err = SJSON_ERR_NO_MATCHING_MEMBER,
    };
}

SjsonResult sjson_object_delete_all(sjson *json, char *key) {
    if (json->type != SJSON_OBJECT)
        return SJSON_ERR_WRONG_TYPE;
    sjson_foreach(json, iter) if (!strcmp(iter->key, key))
        sjson_deletechild(json, iter);
    return SJSON_SUCCESS;
}

SjsonResult sjson_object_set(sjson *json, char *key, sjson *value) {
    if (json->type != SJSON_OBJECT)
        return SJSON_ERR_WRONG_TYPE;
    sjson_object_delete_all(json, key);
    value->key = key;
    sjson_addchild(json, value);
    return SJSON_SUCCESS;
}

SjsonResult sjson_addchild(sjson *json, sjson *child) {
    if (json->type != SJSON_OBJECT && json->type != SJSON_ARRAY)
        return SJSON_ERR_WRONG_TYPE;
    if (json->v.tail == NULL && json->v.child == NULL) {
        json->v.tail = json->v.child = child;
    } else {
        json->v.tail->next = child;
        child->prev = json->v.tail;
        json->v.tail = child;
    }
    return SJSON_SUCCESS;
}

sjson_result sjson_array_get(sjson *json, size_t i) {
    size_t x = 0;
    sjson_foreach(json, iter) if (x++ == i) return (sjson_result){.json = iter};
    return (sjson_result){
        .err = SJSON_ERR_NO_MATCHING_MEMBER,
    };
}

SjsonResult sjson_array_set(sjson *json, size_t i, sjson *x) {
    sjson *it = json->v.child;
    for (size_t j = 0; it; j++) {
        if (j == i) {
            sjson_replace(json, it, x);
            return SJSON_SUCCESS;
        }
        it = it->next;
    }
    return SJSON_ERR_NO_MATCHING_MEMBER;
}

SjsonResult sjson_array_delete(sjson *json, size_t i) {
    sjson *it = json->v.child;
    for (size_t j = 0; it; j++) {
        if (j == i) {
            sjson_deletechild(json, it);
            return SJSON_SUCCESS;
        }
        it = it->next;
    }
    return SJSON_ERR_NO_MATCHING_MEMBER;
}

sjson_result sjson_deserialize(const char *s, size_t len) {
    sjsonlexer lexer;
    sjson_result json;
    sjsonlexer_init(&lexer, s, len);
    SjsonResult lexres = sjsonlexer_lex(&lexer);
    if (lexres != SJSON_SUCCESS)
        return (sjson_result){.err = lexres};
    json = sjson_parse(&lexer.toks);
    free(lexer.toks.a);
    if (json.err)
        return (sjson_result){.err = json.err};
    return json;
}

sjsonbuf sjson_serialize(sjson *json) {
    sjsonbuf s;
    sjsonbuf_init(&s);
    switch (json->type) {
    case SJSON_NUMBER: {
        char buf[1024];
        size_t len = snprintf(buf, sizeof buf, "%lf", json->v.num);
        sjsonbuf_push(&s, buf, len);
        break;
    }
    case SJSON_STRING:
        sjsonbuf_push(&s, "\"", 1);
        for (const char *c = json->v.str; *c; c++) {
            switch (*c) {
            case '\"':
                sjsonbuf_push(&s, "\\\"", 2);
                break;
            case '\n':
                sjsonbuf_push(&s, "\\n", 2);
                break;
            case '\r':
                sjsonbuf_push(&s, "\\r", 2);
                break;
            case '\f':
                sjsonbuf_push(&s, "\\f", 2);
                break;
            case '\t':
                sjsonbuf_push(&s, "\\t", 2);
                break;
            case '\\':
                sjsonbuf_push(&s, "\\\\", 2);
                break;
            case '/':
                sjsonbuf_push(&s, "\\/", 2);
                break;
            default:
                sjsonbuf_push(&s, c, 1);
                break;
            }
        }
        sjsonbuf_push(&s, "\"", 1);
        break;
    case SJSON_NULL:
        sjsonbuf_push(&s, "null", 4);
        break;
    case SJSON_TRUE:
        sjsonbuf_push(&s, "true", 4);
        break;
    case SJSON_FALSE:
        sjsonbuf_push(&s, "false", 5);
        break;
    case SJSON_OBJECT:
        sjsonbuf_push(&s, "{", 1);
        sjson_foreach(json, it) {
            sjsonbuf_push(&s, "\"", 1);
            sjsonbuf_push(&s, it->key, strlen(it->key));
            sjsonbuf_push(&s, "\"", 1);
            sjsonbuf_push(&s, ":", 1);
            sjsonbuf childbuf = sjson_serialize(it);
            sjsonbuf_push(&s, childbuf.buf, childbuf.len);
            free(childbuf.buf);
            if (it->next != NULL)
                sjsonbuf_push(&s, ",", 1);
        }
        sjsonbuf_push(&s, "}", 1);
        break;
    case SJSON_ARRAY:
        sjsonbuf_push(&s, "[", 1);
        sjson_foreach(json, it) {
            sjsonbuf childbuf = sjson_serialize(it);
            sjsonbuf_push(&s, childbuf.buf, childbuf.len);
            free(childbuf.buf);
            if (it->next != NULL)
                sjsonbuf_push(&s, ",", 1);
        }
        sjsonbuf_push(&s, "]", 1);
        break;
    case SJSON_INVALID:
    default:
        break;
    }
    return s;
}

#endif /* SHEEP_SJSON_IMPLEMENTATION */

#ifdef __cplusplus
}
#endif
