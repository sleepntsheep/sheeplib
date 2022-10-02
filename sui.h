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
} sdl_rgba;

typedef struct {
    sdl_rgba bg, fg;
    int btn_pad;
} sui_style;

enum mouse_button {
    MOUSE_LEFT,
    MOUSE_RIGHT,
    MOUSE_MIDDLE,
    MOUSE_BUTTON_COUNT,
};

typedef struct {
    int selected_id;
    int mouse_pressed; /* mouse_button OR'd together */
    int mouse_x, mouse_y;
    sui_style style;
} sui_ctx;

static const sui_style sui_style_default = {
    .bg = {0xff, 0xff, 0xff, 0xff},
    .fg = {0x00, 0x00, 0x00, 0xff},
    .btn_pad = 20,
};

void sui_ctx_init(sui_ctx *ctx);
bool sui_btn(sui_ctx *ctx, const char *label, int x, int y, int id);
inline bool sui_intersect(int x, int y, int w, int h, int mx, int my);

/* these function are left for backend to implement */
void sui_draw_rect(sui_ctx *ctx, int x, int y, int w, int h);
void sui_draw_text(sui_ctx *ctx, const char *text, int x, int y);
/* font is managed by backend, so to draw centered text we need this */
void sui_get_text_size(const char *text, int *w, int *h);

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
    sui_get_text_size(label, &w, &h);
    w += 2 * ctx->style.btn_pad;
    h += 2 * ctx->style.btn_pad;
    int tx = x + ctx->style.btn_pad;
    int ty = y + ctx->style.btn_pad;
    sui_draw_rect(ctx, x, y, w, h);
    sui_draw_text(ctx, label, tx, ty);
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
    else {
        return false;
    }
}

#endif /* SUI_IMPLEMENTATION */

