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
        char *dup = strdup(tests[i]);
        sjson *json = sjson_serialize(dup);
        sjson_deserialize(stdout, json);
        sjson_free(json);
        free(dup);
    }
}
