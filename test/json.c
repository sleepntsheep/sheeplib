#define SHEEP_SJSON_IMPLEMENTATION
#define SHEEP_SJSON_DEBUG 1
#include "../include/sjson.h"
#include <string.h>
#include <stdio.h>


int main() {
    sjson_register_logger(printf);

    sjsontokarr *toks;
    toks = sjson_lex(strdup("{ \"key\": \"value\", \"num\": -2839.489 } "));
    sjson_print_tokarr(toks);
    sjson *json;
    json = sjson_parse(toks);
    sjson_debug_print(json, 0);

    //json = sjson_parse(strdup("{ \"nums\": 12, \"n2\": 390.2, \"arr\": [ 2, 3, 4, \"POG\" ] }"));
    //sjson_debug_print(json.sjson,0);
}
