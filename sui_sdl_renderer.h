#ifndef SUI_SDL_RENDERER_H_
#define SUI_SDL_RENDERER_H_

#include "sui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

static SDL_Renderer *sui_sdl_renderer;
static TTF_Font *sui_sdl_font;

void sui_sdl_start_event();
void sui_sdl_handle_event(sui_ctx *ctx, SDL_Event ev);
void sui_sdl_end_event();
void sui_sdl_init(SDL_Renderer *renderer, TTF_Font *font);

#endif /* SUI_SDL_RENDERER_H_ */

#ifdef SUI_SDL_RENDERER_IMPLEMENTATION

void sui_sdl_start_event() {}
void sui_sdl_end_event() {}

void sui_sdl_init(SDL_Renderer *renderer, TTF_Font *font) {
    sui_sdl_renderer = renderer;
    sui_sdl_font = font;
}

void sui_draw_rect(sui_ctx *ctx, int x, int y, int w, int h) {
    SDL_SetRenderDrawColor(sui_sdl_renderer, ctx->style.bg.r, ctx->style.bg.g, ctx->style.bg.b, ctx->style.bg.a);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(sui_sdl_renderer, &rect);
}

void sui_draw_text(sui_ctx *ctx, const char *text, int x, int y) {
    SDL_Color color = {ctx->style.fg.r, ctx->style.fg.g, ctx->style.fg.b, ctx->style.fg.a};
    SDL_Surface *surface = TTF_RenderText_Solid(sui_sdl_font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(sui_sdl_renderer, surface);
    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(sui_sdl_renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void sui_sdl_handle_event(sui_ctx *ctx, SDL_Event ev) {
    switch (ev.type) {
    case SDL_MOUSEMOTION:
        ctx->mouse_x = ev.motion.x;
        ctx->mouse_y = ev.motion.y;
        break;
    case SDL_MOUSEBUTTONDOWN:
        ctx->mouse_pressed = 1;
        break;
    case SDL_MOUSEBUTTONUP:
        ctx->mouse_pressed = 0;
        break;
    }
}

void sui_get_text_size(const char *text, int *w, int *h) {
    TTF_SizeText(sui_sdl_font, text, w, h);
}

#endif /* SUI_SDL_RENDERER_IMPLEMENTATION */

