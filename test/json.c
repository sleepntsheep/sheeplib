#define SHEEP_SJSON_IMPLEMENTATION
#define SHEEP_SJSON_DEBUG 1
#include "../include/sjson.h"
#include <string.h>
#include <stdio.h>

const char *tests[]=  {
    "{\"obj\": {}}", 
    "[1, 2]",
    "[]",
    "[[]]",
    "{}",
    "[[{}, {}]]",
    "{\"a\": [1,2]}", 
    "[{\"b\": [1,2]}]", 
    "{  \
         \"settings\": {\
            \"dowrap\": true, \
            \"liveupdate\": false, \
            \"pointer\": null\
         },\
         \"delay\": 100.59\
     }"
};

int main() {
    sjson_register_logger(printf);
    for (int i =0  ;i < sizeof tests / sizeof *tests; i++) {
        puts(tests[i]);
        sjsontokarr *toks;
        toks = sjson_lex(strdup(tests[i]));
        sjson_print_tokarr(toks);
        sjson *json;
        json = sjson_parse(toks);
        //sjson_debug_print(json, 0);
        sjson_deserialize(stdout, json);
    }
}
