#include "tetris.h"
static tetramini_data data[TETRAMINI_NUM] = {
    {TETRAMINI_NO_ROTATION, 255, 0, 0, {{0, 0, {{0, 0}, {1, 0}, {0, -1}, {1, -1}}}, {0, 1, {{0, 0}, {1, 0}, {0, -1}, {1, -1}}}, {0, 2, {{0, 0}, {1, 0}, {0, -1}, {1, -1}}}, {0, 3, {{0, 0}, {1, 0}, {0, -1}, {1, -1}}}}},
    {TETRAMINI_ROTATION, 0, 255, 0, {{1, 0, {{0, 0}, {0, -1}, {0, -2}, {0, -3}}}, {1, 1, {{0, 0}, {1, 0}, {2, 0}, {3, 0}}}, {1, 2, {{0, 0}, {0, -1}, {0, -2}, {0, -3}}}, {1, 3, {{0, 0}, {1, 0}, {2, 0}, {3, 0}}}}},
    {TETRAMINI_ROTATION, 125, 125, 125, {{2, 0, {{0, 0}, {0, -1}, {0, -2}, {1, -2}}}, {2, 1, {{0, 0}, {1, 0}, {2, 0}, {2, 1}}}, {2, 2, {{0, 0}, {0, 1}, {0, 2}, {-1, 2}}}, {2, 3, {{0, 0}, {-1, 0}, {-2, 0}, {-2, -1}}}}},
    {TETRAMINI_ROTATION, 0, 0, 255, {{3, 0, {{0, 0}, {0, -1}, {0, -2}, {-1, -2}}}, {3, 1, {{0, 0}, {1, 0}, {2, 0}, {2, -1}}}, {3, 2, {{0, 0}, {0, 1}, {0, 2}, {1, 2}}}, {3, 3, {{0, 0}, {-1, 0}, {-2, 0}, {-2, 1}}}}},
    {TETRAMINI_ROTATION, 200, 100, 50, {{4, 0, {{0, 0}, {1, 0}, {2, 0}, {1, -1}}}, {4, 1, {{0, 0}, {0, 1}, {0, 2}, {1, 1}}}, {4, 2, {{0, 0}, {-1, 0}, {-2, 0}, {-1, 1}}}, {4, 3, {{0, 0}, {0, -1}, {0, -2}, {-1, -1}}}}},
    {TETRAMINI_ROTATION, 50, 200, 100, {{5, 0, {{0, 0}, {0, -1}, {-1, -1}, {-1, -2}}}, {5, 1, {{0, 0}, {1, 0}, {1, -1}, {2, -1}}}, {5, 2, {{0, 0}, {0, 1}, {1, 1}, {1, 2}}}, {5, 3, {{0, 0}, {-1, 0}, {-1, 1}, {-2, 1}}}}},
    {TETRAMINI_ROTATION, 255, 255, 255, {{6, 0, {{0, 0}, {0, -1}, {1, -1}, {1, -2}}}, {6, 1, {{0, 0}, {1, 0}, {1, 1}, {2, 1}}}, {6, 2, {{0, 0}, {0, 1}, {-1, 1}, {-1, 2}}}, {6, 3, {{0, 0}, {-1, 0}, {-1, -1}, {-2, -1}}}}},
};
//TODO: implement
int tetramini_rotate(tetramini *const tetramini_to_rot, const tetris_map *const tetris_map)
{
    tetramini_data *selected_data = data + tetramini_to_rot->type;
    if (selected_data->valid_rot == TETRAMINI_NO_ROTATION)
    {
        return TETRAMINO_BLOCKED;
    }

    int state = tetramini_to_rot->rot_state == TETRAMINI_ROT_STATES - 1 ? 0 : tetramini_to_rot->rot_state + 1;

    tetramino original_pos;
    tetramino_copy(&original_pos, tetramini_to_rot->arr_tetramini);

    tetramini temp;
    tetramini_copy(&temp, selected_data->tetramini_init + state);

    register int i;
    for (i = 0; i < TETRAMINI_SIZE; i++)
    {
        tetramino *tet = temp.arr_tetramini + i;
        tet->x += original_pos.x;
        tet->y += original_pos.y;
        const int index = tetris_map->width * tet->y + tet->x;

        if (IS_EXCLUDED(tet->x, 0, tetris_map->width - 1) || tet->y >= tetris_map->height || (index >= 0 && tetris_map->cells[index]))
        {
            return TETRAMINO_BLOCKED;
        }
    }

    tetramini_copy(tetramini_to_rot, &temp);

    return TETRAMINO_OK;
}
int can_tetramini_move_down(const tetramini *const tetramini, const tetris_map *const tetris_map)
{
    int result;
    register int i;
    for (i = 0; i < TETRAMINI_SIZE; i++)
    {
        result = can_tetramino_move_down(tetramini->arr_tetramini + i, tetris_map);
        if (result)
        {
            return result;
        }
    }
    return result;
}
int can_tetramini_move_left(const tetramini *const tetramini, const tetris_map *const tetris_map)
{
    int result;
    register int i;
    for (i = 0; i < TETRAMINI_SIZE; i++)
    {
        result = can_tetramino_move_left(tetramini->arr_tetramini + i, tetris_map);
        if (result)
        {
            return result;
        }
    }
    return result;
}
int can_tetramini_move_right(const tetramini *const tetramini, const tetris_map *const tetris_map)
{
    int result;
    register int i;
    for (i = 0; i < TETRAMINI_SIZE; i++)
    {
        result = can_tetramino_move_right(tetramini->arr_tetramini + i, tetris_map);
        if (result)
        {
            return result;
        }
    }
    return result;
}
int tetramini_move_down(tetramini *const tetramini, tetris_map *const tetris_map)
{
    int result = can_tetramini_move_down(tetramini, tetris_map);
    register int i;
    if (result == TETRAMINO_STOPPED)
    {
        for (i = 0; i < TETRAMINI_SIZE; i++)
        {
            const int index = tetris_map->width * tetramini->arr_tetramini[i].y + tetramini->arr_tetramini[i].x;
            if (IS_INCLUDED_INCLUSIVE(index, 0, tetris_map->width * tetris_map->height - 1))
            {
                tetris_map->cells[index] = 1;
            }
        }
        return result;
    }
    for (i = 0; i < TETRAMINI_SIZE; i++)
    {
        tetramino_move_down(tetramini->arr_tetramini + i, tetris_map);
    }
    return result;
}
int tetramini_move_left(tetramini *const tetramini, const tetris_map *const tetris_map)
{
    int result = can_tetramini_move_left(tetramini, tetris_map);
    if (result)
    {
        return result;
    }
    register int i;
    for (i = 0; i < TETRAMINI_SIZE; i++)
    {
        result = tetramino_move_left(tetramini->arr_tetramini + i, tetris_map);
        if (result)
        {
            return result;
        }
    }
    return result;
}
int tetramini_move_right(tetramini *const tetramini, const tetris_map *const tetris_map)
{
    int result = can_tetramini_move_right(tetramini, tetris_map);
    if (result)
    {
        return result;
    }
    register int i;
    for (i = 0; i < TETRAMINI_SIZE; i++)
    {
        result = tetramino_move_right(tetramini->arr_tetramini + i, tetris_map);
        if (result)
        {
            return result;
        }
    }
    return result;
}
void tetramini_init(tetramini *const tetramini, const tetris_map *const tetris_map, int type)
{
    if (type < 0 || type >= TETRAMINI_NUM)
    {
        type = precise_rand_lim(TETRAMINI_NUM - 1);
    }

    tetramini_copy(tetramini, (data + type)->tetramini_init);
    const int x_offset = quick_rand_range(TETRAMINI_SIZE - 1, tetris_map->width - TETRAMINI_SIZE);
    const int y_offset = TETRAMINO_INITIAL_HEIGHT;

    register int i;
    for (i = 0; i < TETRAMINI_SIZE; i++)
    {
        tetramini->arr_tetramini[i].x += x_offset;
        tetramini->arr_tetramini[i].y += y_offset;
    }
}
tetramini_data *get_type_data(const int tetramini_type)
{
    return data + tetramini_type;
}
void tetramini_destroy(tetramini *const tetramini_to_destroy)
{
    memset(tetramini_to_destroy, 0, sizeof(tetramini));
}
void tetramini_copy(tetramini *const to_fill, const tetramini *const to_copy)
{
    to_fill->rot_state = to_copy->rot_state;
    to_fill->type = to_copy->type;

    memcpy(to_fill->arr_tetramini, to_copy->arr_tetramini, TETRAMINI_SIZE * sizeof(tetramino));
}
void tetramino_init(tetramino *const tetramino, const tetris_map *const tetris_map)
{
    tetramino->y = TETRAMINO_INITIAL_HEIGHT;
    tetramino->x = quick_rand_range(0, tetris_map->width - 1);
}
void tetramino_copy(tetramino *const to_fill, const tetramino *const to_copy)
{
    memcpy(to_fill, to_copy, sizeof(tetramino));
}
void tetramino_destroy(tetramino *const tetramino_to_destroy)
{
    memset(tetramino_to_destroy, 0, sizeof(tetramino));
}
int is_line_filled(const tetris_map *const tetris_map, const int line)
{
    register int x;
    int mult = tetris_map->width * (line - 1);
    for (x = 0; x < tetris_map->width; x++)
    {
        if (tetris_map->cells[mult + x] == 0)
        {
            return TETRIS_MAP_LINE_NOT_FILLED;
        }
    }
    return TETRIS_MAP_LINE_FILLED;
}
int free_filled_lines(tetris_map *const tetris_map, const int starting_line, const int ending_line)
{
    register int i;
    int result = 0;
    for (i = starting_line; i <= ending_line; i++)
    {
        if (is_line_filled(tetris_map, i))
        {
            free_line(tetris_map, i);
            result++;
        }
    }
    return result;
}
void free_line(tetris_map *const tetris_map, const int line)
{
    if (line > 1)
    {
        size_t size = tetris_map->width * (line - 1) * sizeof(int);
        memmove(tetris_map->cells + tetris_map->width, tetris_map->cells, size);
    }
    memset(tetris_map->cells, 0, sizeof(int) * tetris_map->width);
}
int tetramino_move_down(tetramino *const tetramino, tetris_map *const tetris_map)
{
    const signed int curr_index = tetris_map->width * tetramino->y + tetramino->x;
    const signed int can_move_result = can_tetramino_move_down(tetramino, tetris_map);

    if (can_move_result == TETRAMINO_OK)
    {
        tetramino->y++;
    }
    else if (can_move_result == TETRAMINO_STOPPED && IS_INCLUDED_INCLUSIVE(curr_index, 0, tetris_map->width * tetris_map->height - 1))
    {
        tetris_map->cells[curr_index] = 1;
    }
    return can_move_result;
}
int tetramino_move_left(tetramino *const tetramino, const tetris_map *const tetris_map)
{
    const signed int can_move_result = can_tetramino_move_left(tetramino, tetris_map);
    if (can_move_result == TETRAMINO_OK)
    {
        tetramino->x--;
    }
    return can_move_result;
}
int tetramino_move_right(tetramino *const tetramino, const tetris_map *const tetris_map)
{
    const signed int can_move_result = can_tetramino_move_right(tetramino, tetris_map);
    if (can_move_result == TETRAMINO_OK)
    {
        tetramino->x++;
    }
    return can_move_result;
}

int can_tetramino_move_down(const tetramino *const tetramino, const tetris_map *const tetris_map)
{
    const signed int next_index = tetris_map->width * (tetramino->y + 1) + tetramino->x;

    if ((tetramino->y + 1) >= tetris_map->height || (next_index >= 0 && tetris_map->cells[next_index]))
    {
        return TETRAMINO_STOPPED;
    }
    return TETRAMINO_OK;
}
int can_tetramino_move_left(const tetramino *const tetramino, const tetris_map *const tetris_map)
{
    const signed int next_index = tetris_map->width * tetramino->y + tetramino->x - 1;

    if ((tetramino->x - 1) < 0 || (next_index >= 0 && tetris_map->cells[next_index]))
    {
        return TETRAMINO_BLOCKED;
    }
    return TETRAMINO_OK;
}
int can_tetramino_move_right(const tetramino *const tetramino, const tetris_map *const tetris_map)
{
    const signed int next_index = tetris_map->width * tetramino->y + tetramino->x + 1;

    if ((tetramino->x + 1) >= tetris_map->width || (next_index >= 0 && tetris_map->cells[next_index]))
    {
        return TETRAMINO_BLOCKED;
    }
    return TETRAMINO_OK;
}
void tetris_map_init(tetris_map *const tetris_map_to_init, const Uint32 height, const Uint32 width)
{
    memset(tetris_map_to_init, 0, sizeof(tetris_map));
    const size_t mem_size = sizeof(int) * height * width;

    tetris_map_to_init->cells = malloc(mem_size);
    memset(tetris_map_to_init->cells, 0, mem_size);

    tetris_map_to_init->height = height;
    tetris_map_to_init->width = width;
}
void tetris_map_destroy(tetris_map *const tetris_map_to_destroy)
{
    const size_t mem_size = sizeof(int) * tetris_map_to_destroy->height * tetris_map_to_destroy->width;

    memset(tetris_map_to_destroy->cells, 0, mem_size);
    free(tetris_map_to_destroy->cells);
    memset(tetris_map_to_destroy, 0, sizeof(tetris_map));
}
