#ifndef SUI_H_
#define SUI_H_

/*
 * hashing widget's label for id might be simpler for library user,
 * but i don't care and that is hard to implement 😈😈😈😈
 * so user will have to give id to each widget themselves
 *
 * TODOOOOOOO: more widgets like textlabel, radiobox
 */

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t r, g, b, a;
} sui_rgba;

typedef struct {
    sui_rgba bg, bg2, fg, fg2;
    int btn_pad;
} sui_style;

enum mouse_button {
    MOUSE_LEFT,
    MOUSE_RIGHT,
    MOUSE_MIDDLE,
    MOUSE_BUTTON_COUNT,
};

typedef struct sui_ctx_s sui_ctx;

struct sui_ctx_s {
    int selected_id;
    int mouse_pressed; /* mouse_button OR'd together */
    int mouse_x, mouse_y;
    sui_style style;
    void (*draw_rect)(sui_ctx *, int, int, int, int, sui_rgba);
    void (*draw_text)(sui_ctx *, const char *text, int, int, sui_rgba);
    void (*get_text_size)(const char *text, int *w, int *h);
};

static const sui_style sui_style_default = {
    .bg = {0xff, 0xff, 0xff, 0xff},
    .bg2 = {0xaa, 0xaa, 0xaa, 0xff},
    .fg = {0x00, 0x00, 0x00, 0xff},
    .fg2 = {0x00, 0xff, 0x00, 0xff},
    .btn_pad = 5,
};

void sui_ctx_init(sui_ctx *ctx);
bool sui_btn(sui_ctx *ctx, const char *label, int x, int y, int id);
void sui_text(sui_ctx *ctx, const char *label, int x, int y);
inline bool sui_intersect(int x, int y, int w, int h, int mx, int my);
bool sui_slider_double(sui_ctx *ctx, double vmin, double *value, double vmax,
                       int x, int y, int w, int h);
bool sui_slider_label_double(sui_ctx *ctx, const char *label, double vmin,
                             double *value, double vmax, int x, int y, int w);
bool sui_slider_int(sui_ctx *ctx, int vmin, int *value, int vmax, int x, int y,
                    int w, int h);
bool sui_slider_label_int(sui_ctx *ctx, const char *label, int vmin, int *value,
                          int vmax, int x, int y, int w);
bool sui_checkbox_label(sui_ctx *ctx, bool value, const char *label, int x,
                        int y, int id);

#endif /* SUI_H_ */

#ifdef SUI_IMPLEMENTATION

inline bool sui_intersect(int x, int y, int w, int h, int mx, int my) {
    return mx >= x && mx <= x + w && my >= y && my <= y + h;
}

void sui_ctx_init(sui_ctx *ctx) {
    ctx->selected_id = -1;
    ctx->mouse_pressed = 0;
    ctx->style = sui_style_default;
}

bool sui_btn(sui_ctx *ctx, const char *label, int x, int y, int id) {
    int w, h;
    ctx->get_text_size(label, &w, &h);
    w += 2 * ctx->style.btn_pad;
    h += 2 * ctx->style.btn_pad;
    int tx = x + ctx->style.btn_pad;
    int ty = y + ctx->style.btn_pad;
    ctx->draw_rect(ctx, x, y, w, h, ctx->style.bg);
    ctx->draw_text(ctx, label, tx, ty, ctx->style.fg);
    if (!ctx->mouse_pressed) {
        ctx->selected_id = -1;
        return false;
    }
    /* only trigger once (KEYDOWN or KEYUP), not between */
    /* why is it triggering multiple time hep hep hep */
    if (ctx->selected_id == id)
        return false;
    if (sui_intersect(x, y, w, h, ctx->mouse_x, ctx->mouse_y)) {
        ctx->selected_id = id;
        return true;
    }
    return false;
}

bool sui_slider_double(sui_ctx *ctx, double vmin, double *value, double vmax,
                       int x, int y, int w, int h) {
    double percent = *value / (vmax - vmin);
    int offset = percent * w;
    ctx->draw_rect(ctx, x, y, w, h, ctx->style.fg2);
    ctx->draw_rect(ctx, x + offset, y, w - offset, h, ctx->style.bg2);
    if (!ctx->mouse_pressed)
        return false;
    if (sui_intersect(x, y, w, h, ctx->mouse_x, ctx->mouse_y)) {
        *value = (vmax - vmin) * (double)(ctx->mouse_x - x) / w;
        return true;
    }
    return false;
}

bool sui_slider_label_double(sui_ctx *ctx, const char *label, double vmin,
                             double *value, double vmax, int x, int y, int w) {
    int tw, h;
    ctx->get_text_size(label, &tw, &h);
    bool result = sui_slider_double(ctx, vmin, value, vmax, x, y, w, h);
    ctx->draw_text(ctx, label, x + (w - tw) / 2, y, ctx->style.fg);
    return result;
}

bool sui_slider_int(sui_ctx *ctx, int vmin, int *value, int vmax, int x, int y,
                    int w, int h) {
    double percent = (double)*value / (vmax - vmin);
    int offset = percent * w;
    ctx->draw_rect(ctx, x, y, w, h, ctx->style.fg2);
    ctx->draw_rect(ctx, x + offset, y, w - offset, h, ctx->style.bg2);
    if (!ctx->mouse_pressed)
        return false;
    if (sui_intersect(x, y, w, h, ctx->mouse_x, ctx->mouse_y)) {
        *value = round((vmax - vmin) * (double)(ctx->mouse_x - x) / w);
        return true;
    }
    return false;
}

bool sui_slider_label_int(sui_ctx *ctx, const char *label, int vmin, int *value,
                          int vmax, int x, int y, int w) {
    int tw, h;
    ctx->get_text_size(label, &tw, &h);
    bool result = sui_slider_int(ctx, vmin, value, vmax, x, y, w, h);
    ctx->draw_text(ctx, label, x + (w - tw) / 2, y, ctx->style.fg);
    return result;
}

bool sui_checkbox_label(sui_ctx *ctx, bool value, const char *label, int x,
                        int y, int id) {
    int size;
    ctx->get_text_size(label, NULL, &size);
    ctx->draw_rect(ctx, x, y, size, size,
                   value ? ctx->style.fg2 : ctx->style.bg2);
    ctx->draw_text(ctx, label, x + size, y, ctx->style.fg2);
    if (!ctx->mouse_pressed) {
        ctx->selected_id = -1;
        return value;
    }
    if (ctx->selected_id == id)
        return value;
    if (sui_intersect(x, y, size, size, ctx->mouse_x, ctx->mouse_y)) {
        ctx->selected_id = id;
        return !value;
    }
    return value;
}

void sui_text(sui_ctx *ctx, const char *label, int x, int y) {
    int w, h;
    ctx->get_text_size(label, &w, &h);
    w += 2 * ctx->style.btn_pad;
    h += 2 * ctx->style.btn_pad;
    int tx = x + ctx->style.btn_pad;
    int ty = y + ctx->style.btn_pad;
    ctx->draw_rect(ctx, x, y, w, h, ctx->style.bg);
    ctx->draw_text(ctx, label, tx, ty, ctx->style.fg);
}

#undef SUI_IMPLEMENTATION
#endif /* SUI_IMPLEMENTATION */
