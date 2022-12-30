#ifndef SHEEP_RENDERER_H
#define SHEEP_RENDERER_H
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

struct Renderer {
    uint32_t *fb;
    int width;
    int height;
};

void renderer_init(struct Renderer *, int width, int height);
void renderer_cleanup(struct Renderer *);
void renderer_fill(struct Renderer *, uint32_t color);
void renderer_put_point(struct Renderer *, int x, int y, uint32_t color);
void renderer_put_rect(struct Renderer *, int x, int y, int w, int h, uint32_t color);
void renderer_put_circle(struct Renderer *, int x, int y, int r, uint32_t color);
void renderer_put_line(struct Renderer *, int x1, int y1, int x2, int y2, int size, uint32_t color);
void renderer_save_ppm(struct Renderer *, const char *);

#endif /* SHEEP_RENDERER_H */

#ifdef SHEEP_RENDERER_IMPLEMENTATION

#include <string.h>

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif
#ifndef ABS
#define ABS(a) ((a) < 0 ? (-(a)) : (a))
#endif



void renderer_init(struct Renderer *r, int width, int height)
{
    r->width = width;
    r->height = height;
    r->fb = malloc(sizeof r->fb[0] * width * height);
}

void renderer_cleanup(struct Renderer *r)
{
    free(r->fb);
}

void renderer_put_point(struct Renderer *r, int x, int y, uint32_t color)
{
    if (x < 0 || y < 0 || x >= r->width || y >= r->height) return;
    r->fb[y * r->width + x] = color;
}

void renderer_put_rect(struct Renderer *r, int x, int y, int w, int h, uint32_t color)
{
    for (int i = MAX(y, 0); i < r->height && i < y + h; i++)
    {
        for (int j = MAX(x, 0); j < r->width && j < x + w; j++)
        {
            r->fb[i * r->width + j] = color;
        }
    }
}

void renderer_fill(struct Renderer *r, uint32_t color)
{
    renderer_put_rect(r, 0, 0, r->width, r->height, color);
}

void renderer_put_line(struct Renderer *r, int x1, int y1, int x2, int y2, int size, uint32_t color)
{
    int dx = ABS(x2 - x1);
    int sx = x1 < x2 ? 1 : -1;
    int dy = -ABS(y2 - y1);
    int sy = y1 < y2 ? 1 : -1;
    int err = dx + dy;
    int e2;
    for (;;)
    {
        for (int i = -size; i < size; i++)
        {
            for (int j = -size; j < size; j++)
            {
                if (x1 + i < 0 || x1 + i >= r->width ||
                        y1 + j < 0 || y1 + j >= r->height)
                {
                    continue;
                }
                renderer_put_point(r, x1 + i, y1 + j, color);
            }
        }
        if (x1 == x2 && y1 == y2)
        {
            break;
        }
        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void renderer_put_circle(struct Renderer *r, int x, int y, int radius, uint32_t color)
{

}

void renderer_save_ppm(struct Renderer *r, const char *path)
{
    FILE *fp = fopen(path, "w");
    if (!fp) return;
    fprintf(fp, "P6 %d %d 255\n", r->width, r->height);
    for (int i = 0; i < r->width * r->height; i++)
    {
        uint32_t color = r->fb[i];
        fputc((color >> 24) & 0xff, fp);
        fputc((color >> 16) & 0xff, fp);
        fputc((color >> 8) & 0xff, fp);
    }
}

#endif /* SHEEP_RENDERER_IMPLEMENTATION */

