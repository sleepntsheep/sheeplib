#ifndef SHEEP_SJSON_H
#define SHEEP_SJSON_H

enum json_types {
    JSON_INVALID,
    JSON_FALSE,
    JSON_TRUE,
    JSON_NULL,
    JSON_NUMBER,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT,
};

#define JSON_DOUBLE_QUOTE_TOKEN '"'
#define JSON_TRUE_TOKEN "true"
#define JSON_FALSE_TOKEN "false"
#define JSON_TRUE_TOKEN_LEN 4
#define JSON_FALSE_TOKEN_LEN 5
#define JSON_NULL_TOKEN "null"
#define JSON_NULL_TOKEN_LEN 4
#define JSON_WHITESPACE_TOKEN " "
#define JSON_SYNTAX_TOKEN "{}[],"

typedef struct json_object_s {
    struct json_object_s *prev;
    struct json_object_s *next;

    int type;
    char *string;
    double number;
    char *name;
} json_object_t;

json_object_t json_lex(char *s);
json_object_t json_object_new();

#endif /* SHEEP_SJSON_H */

#ifdef SHEEP_SJSON_IMPLEMENTATION

typedef struct json_lex_return_s {
    json_object_t* object;
    char* string;
} json_lex_return_t;

json_object_t* json_object_new(int type) {
    json_object_t* obj = malloc(sizeof *obj);
    obj->prev = obj->next = NULL;
}

json_lex_return_t json_lex_null(char *s) {
    int type = JSON_INVALID;
    if (strncmp(s, JSON_NULL_TOKEN,
                JSON_NULL_TOKEN_LEN) == 0) {
        type = JSON_NULL;
        s += JSON_NULL_TOKEN_LEN;
    }
    return (json_lex_return_t) {
        json_object_new(type), s 
    };
}

json_lex_return_t json_lex_bool(char *s) {
    int type = JSON_INVALID;
    if (strncmp(s, JSON_TRUE_TOKEN,
                JSON_TRUE_TOKEN_LEN) == 0) {
        type = JSON_TRUE;
        s += JSON_TRUE_TOKEN_LEN;
    }
    else if (strncmp(s, JSON_FALSE_TOKEN,
                JSON_FALSE_TOKEN_LEN) == 0) {
        type = JSON_FALSE;
        s += JSON_FALSE_TOKEN_LEN;
    }
    return (json_lex_return_t) {
        json_object_new(type), s 
    };
}

json_lex_return_t json_lex_string(char *s) {
    char *original_s = s;
    int isvalidstring = 1;
    char *stringvalue = NULL;
    size_t length = 0, stringpt = 0;
    char *rightquote = NULL;
    if (*s != JSON_DOUBLE_QUOTE_TOKEN) {
        isvalidstring = 0;
    }
    if ((rightquote = strchr(s+1, JSON_DOUBLE_QUOTE_TOKEN)) == NULL) {
        isvalidstring = 0;
    }
    length = rightquote - s;
    stringvalue = malloc(length);
    for (;s != rightquote;) {
        stringvalue[stringpt++] = *s;
    }
done:;
     if (!isvalidstring) {
         return (json_lex_return_t) {
             json_object_new(JSON_INVALID),
             original_s,
         }
     }
    json_object_t* obj = json_object_new(JSON_STRING);
    obj->string = stringvalue;
    return (json_lex_return_t) { obj, s; }
}

json_lex_return_t json_lex_number(char *s) {
    char *original_s = s;
    double num = 0;
    int isnegative = 0;
    int isvalidnum = 1;
    for (;s;) {
        switch (*s) {
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':
                num *= 10;
                num += *s - '0';
                break;
            case '-':
                if (num == 0) {
                    isnegative = 1;
                } else {
                }
                break;
            default:
                if (num == 0) {
                    isvalidnum = 0;
                    goto done;
                }
                break;
        }
    }
done:;
    if (!isvalidnum) {
        return (json_lex_return_t) {
            json_object_new(JSON_INVALID),
            original_s,
        };
    }
    json_object_t* obj = json_object_new(JSON_NUMBER);
    obj->number = num * isnegative;
    return (json_lex_return_t) { obj, s };
}

json_object_t json_lex(char *s) {
    json_object_t* tokens = json_object_new(json_object);
    json_lex_return_t lex_return;

    for(;s;) {
        lex_return = json_lex_number(s);
        s = lex_return.string;
        if (lex_return.object->type != JSON_INVALID)
            tokens = tokens->next = lex_return.object
        lex_return = json_lex_string(s);
        s = lex_return.string;
        if (lex_return.object->type != JSON_INVALID)
            tokens = tokens->next = lex_return.object
        lex_return = json_lex_bool(s);
        s = lex_return.string;
        if (lex_return.object->type != JSON_INVALID)
            tokens = tokens->next = lex_return.object
        lex_return = json_lex_null(s);
        s = lex_return.string;
        if (lex_return.object->type != JSON_INVALID)
            tokens = tokens->next = lex_return.object

        if (strchr(JSON_WHITESPACE_TOKEN, *s))
            s++;
        else if (strchr(JSON_SYNTAX_TOKEN, *s))
            tokens = tokens->next = 
    }
}

#endif /* SHEEP_SJSON_IMPLEMENTATION */

