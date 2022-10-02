#ifndef SUI_NCURSES_RENDERER_H_
#define SUI_NCURSES_RENDERER_H_
#include <ncurses.h>
/* ncurses already provide global state (stdscr) */

#endif /* SUI_NCURSES_RENDERER_H_ */

#ifdef SUI_NCURSES_RENDERER_IMPLEMENTATION

/* TODOOOOOOOOOOOOOOOOOOO: HOW DO I MAKE SUI SUPPORT MULTIPLE BACKEND IN SAME COMPILATION UNIT */

void
tui_button_draw(struct tui_button btn)
{
    /* we can't draw rgba here, so we just use A_REVERSE */
    attron(A_REVERSE);
    for (int i = 0; i < btn.h; i++) {
        mvprintw(i+btn.y, btn.x, "%.*s", btn.w, "                                                                                                ");
        if (i == btn.h / 2)
            mvprintw(i+btn.y, btn.x, "%.*s", btn.w, btn.text);
    }
    attroff(A_REVERSE);
}

#endif /* SUI_NCURSES_RENDERER_IMPLEMENTATION */

