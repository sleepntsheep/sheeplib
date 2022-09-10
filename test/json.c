#define SHEEP_SJSON_IMPLEMENTATION
#include "../include/json.h"
#include <string.h>
#include <stdio.h>
#define SHEEP_SJSON_DEBUG 1

int main() {
    sjsonwiths json;
    json = sjson_parse(strdup("\"string\""));
    sjson_debug_print(json.sjson,0);
    json = sjson_parse(strdup("294"));
    sjson_debug_print(json.sjson,0);
    json = sjson_parse(strdup("294.90"));
    sjson_debug_print(json.sjson,0);
    char j[] = " { \"key\": \"value\", } ";
    json = sjson_parse(j);
    sjson_debug_print(json.sjson,0);
}
