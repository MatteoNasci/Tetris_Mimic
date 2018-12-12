#include "gfx.h"
#include "utils.h"
#define FILL_RECT(rect_name, x1, y1, h1, w1) \
    rect_name.x = x1;                        \
    rect_name.y = y1;                        \
    rect_name.h = h1;                        \
    rect_name.w = w1;

void set_color_rgba_8_from_tetramini_type(color_rgba_8 *const to_fill, const int type)
{
    const tetramini_data *const selected_data = get_type_data(type);
    set_color_rgba_8(to_fill, selected_data->color_r, selected_data->color_g, selected_data->color_b, 255);
}
int tetramini_draw(const tetramini *const tetramini, SDL_Renderer *const renderer, const int sqare_size)
{
    register int i;
    int result = 0;
    color_rgba_8 color;
    set_color_rgba_8_from_tetramini_type(&color, tetramini->type);
    for (i = 0; i < TETRAMINI_SIZE; i++)
    {
        result = tetramino_draw(tetramini->arr_tetramini + i, renderer, sqare_size, &color);
        if (result)
        {
            return result;
        }
    }
    return result;
}
int tetramini_draw_offset(const tetramini *const tetramini, SDL_Renderer *const renderer, const int sqare_size, const SDL_Rect *const origin)
{
    register int i;
    int result = 0;
    color_rgba_8 color;
    set_color_rgba_8_from_tetramini_type(&color, tetramini->type);
    tetramini_data *selected_data = get_type_data(tetramini->type);
    for (i = 0; i < TETRAMINI_SIZE; i++)
    {
        SDL_Rect rect = *origin;
        rect.x += (selected_data->tetramini_init->arr_tetramini + i)->x * sqare_size;
        rect.y += (selected_data->tetramini_init->arr_tetramini + i)->y * sqare_size;
        result = _tetris_draw_internal(renderer, &rect, &color);

        if (result)
        {
            return result;
        }
    }
    return result;
}
void color_rgba_8_copy(color_rgba_8 *const to_fill, const color_rgba_8 *const to_copy)
{
    memcpy(to_fill, to_copy, sizeof(color_rgba_8));
}
void set_color_rgba_8(color_rgba_8 *const color, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    color->r = r;
    color->g = g;
    color->b = b;
    color->a = a;
}
void set_random_color_rgba_8(color_rgba_8 *const color, const Uint8 min, const Uint8 max, Uint8 a)
{
    color->r = quick_rand_range(min, max);
    color->g = quick_rand_range(min, max);
    color->b = quick_rand_range(min, max);
    color->a = a;
}
int _tetris_draw_internal(SDL_Renderer *const renderer, const SDL_Rect *const rect, const color_rgba_8 *const color)
{
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
    return SDL_RenderFillRect(renderer, rect);
}
int tetramino_draw(const tetramino *const tetramino, SDL_Renderer *const renderer, const int square_size, const color_rgba_8 *const color)
{
    SDL_Rect rect;
    FILL_RECT(rect, (square_size * tetramino->x), (square_size * tetramino->y), square_size, square_size);

    return _tetris_draw_internal(renderer, &rect, color);
}
int tetris_map_draw(const tetris_map *const tetris_map, SDL_Renderer *const renderer, const int square_size, const color_rgba_8 *const color)
{
    int x, y;
    SDL_Rect rect;
    int result = 0;
    for (x = 0; x < tetris_map->width; x++)
    {
        for (y = 0; y < tetris_map->height; y++)
        {
            int index = tetris_map->width * y + x;
            if (tetris_map->cells[index])
            {
                FILL_RECT(rect, (square_size * x), (square_size * y), square_size, square_size);
                result = _tetris_draw_internal(renderer, &rect, color);
                if (result)
                {
                    return result;
                }
            }
        }
    }
    return result;
}