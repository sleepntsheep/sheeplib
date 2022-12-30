#define SHEEP_RENDERER_IMPLEMENTATION
#include "../renderer.h"

int main() {
    struct Renderer r;
    renderer_init(&r, 800, 600);
    renderer_fill(&r, 0x000000FF);
    renderer_put_rect(&r, 100, 100, 200, 100, 0xFFFFFFFF);
    renderer_save_ppm(&r, "render.ppm");
    renderer_cleanup(&r);
    return 0;
}

