#include "ui/ui.h"
#include <stdio.h>

bool ui_init(UIManager *ui, const char *font_path, int font_size) {
  if (!TTF_Init()) {
    printf("Erro ao inicializar TTF: %s\n", SDL_GetError());
    return false;
  }

  ui->main_font = TTF_OpenFont(font_path, font_size);
  if (!ui->main_font) {
    printf("Erro ao carregar fonte: %s\n", SDL_GetError());
    return false;
  }

  ui->color_white = (SDL_Color){255, 255, 255, 255};
  ui->color_green = (SDL_Color){0, 255, 0, 255};

  return true;
}

void ui_cleanup(UIManager *ui) {
  if (ui->main_font) {
    TTF_CloseFont(ui->main_font);
  }
  TTF_Quit();
}

void ui_draw_text(UIManager *ui, SDL_Renderer *renderer, const char *text,
                  float x, float y, SDL_Color color) {
  if (!ui->main_font || !text)
    return;

  // 1. Cria uma surface com o texto renderizado
  SDL_Surface *surface = TTF_RenderText_Blended(ui->main_font, text, 0, color);
  if (!surface)
    return;

  // 2. Converte a surface em uma textura que a placa de vídeo entende
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_DestroySurface(surface);
  if (!texture)
    return;

  // 3. Define as dimensões com base no tamanho do texto gerado
  SDL_FRect dest_rect;
  dest_rect.x = x;
  dest_rect.y = y;

  // Pega o tamanho real da textura gerada
  float w, h;
  SDL_GetTextureSize(texture, &w, &h);
  dest_rect.w = w;
  dest_rect.h = h;

  SDL_RenderTexture(renderer, texture, NULL, &dest_rect);
  SDL_DestroyTexture(texture);
}
