#define SHEEP_SJSON_IMPLEMENTATION
#include "../sjson.h"
#include <iostream>

int main() {
    sjson_result j = sjson_deserialize("{}", 2);
    if (j.err) {
        std::cerr << "Failed parsing json" << std::endl;
    }
    sjson_free(j.json);
}
