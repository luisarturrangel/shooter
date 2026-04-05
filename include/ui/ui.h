#ifndef UI_H
#define UI_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>

typedef struct {
  TTF_Font *main_font;
  SDL_Color color_white;
  SDL_Color color_green;
} UIManager;

bool ui_init(UIManager *ui, const char *font_path, int font_size);
void ui_cleanup(UIManager *ui);

void ui_draw_text(UIManager *ui, SDL_Renderer *renderer, const char *text,
                  float x, float y, SDL_Color color);

#endif // UI_H
