#ifndef GFX_H_
#define GFX_H_

#include "tetris.h"

typedef struct color_rgba_8
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
} color_rgba_8;

void set_color_rgba_8_from_tetramini_type(color_rgba_8 *const to_fill, const int type);
void color_rgba_8_copy(color_rgba_8 *const to_fill, const color_rgba_8 *const to_copy);
void set_color_rgba_8(color_rgba_8 *const color, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void set_random_color_rgba_8(color_rgba_8 *const color, const Uint8 min, const Uint8 max, Uint8 a);
int tetramino_draw(const tetramino *const tetramino, SDL_Renderer *const renderer, const int sqare_size, const color_rgba_8 *const color);
int tetramini_draw(const tetramini *const tetramini, SDL_Renderer *const renderer, const int sqare_size);
int tetramini_draw_offset(const tetramini *const tetramini, SDL_Renderer *const renderer, const int sqare_size, const SDL_Rect *const origin);
int tetris_map_draw(const tetris_map *const tetramino, SDL_Renderer *const renderer, const int sqare_size, const color_rgba_8 *const color);
int _tetris_draw_internal(SDL_Renderer *const renderer, const SDL_Rect *const rect, const color_rgba_8 *const color);
#endif