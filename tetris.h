#ifndef TETRIS_H_
#define TETRIS_H_
#define SDL_MAIN_HANDLED
#include <string.h>
#include <stdlib.h>
#include <SDL.h>
#include "utils.h"

#define TETRAMINO_INITIAL_HEIGHT -1
#define TETRAMINO_OK 0
#define TETRAMINO_STOPPED 1
#define TETRAMINO_BLOCKED 2

#define TETRIS_MAP_LINE_FILLED 1
#define TETRIS_MAP_LINE_NOT_FILLED 0

#define TETRAMINI_NUM 7
#define TETRAMINI_ROT_STATES 4
#define TETRAMINI_SIZE 4
#define TETRAMINI_NO_ROTATION 1
#define TETRAMINI_ROTATION 0

typedef struct tetramino
{
    int x;
    int y;
} tetramino;

typedef struct tetramini
{
    unsigned char type : 5;
    unsigned char rot_state : 3;
    tetramino arr_tetramini[TETRAMINI_SIZE];
} tetramini;

typedef struct tetris_map
{
    Sint32 height;
    Sint32 width;
    int *cells;
} tetris_map;

typedef struct tetramini_data
{
    unsigned int valid_rot : 1;
    Uint8 color_r;
    Uint8 color_g;
    Uint8 color_b;
    tetramini tetramini_init[TETRAMINI_ROT_STATES];
} tetramini_data;

extern tetramini_data data[];

tetramini_data *get_type_data(const int tetramini_type);
int tetramini_rotate(tetramini *const tetramini, const tetris_map *const tetris_map);
int can_tetramini_move_down(const tetramini *const tetramini, const tetris_map *const tetris_map);
int can_tetramini_move_left(const tetramini *const tetramini, const tetris_map *const tetris_map);
int can_tetramini_move_right(const tetramini *const tetramini, const tetris_map *const tetris_map);
int tetramini_move_down(tetramini *const tetramini, tetris_map *const tetris_map);
int tetramini_move_left(tetramini *const tetramini, const tetris_map *const tetris_map);
int tetramini_move_right(tetramini *const tetramini, const tetris_map *const tetris_map);
void tetramini_init(tetramini *const tetramini, const tetris_map *const tetris_map, int type);
void tetramini_destroy(tetramini *const tetramini);
void tetramini_copy(tetramini *const to_fill, const tetramini *const to_copy);
void tetramino_copy(tetramino *const to_fill, const tetramino *const to_copy);
int is_line_filled(const tetris_map *const tetris_map, const int line);
int free_filled_lines(tetris_map *const tetris_map, const int starting_line, const int ending_line);
void free_line(tetris_map *const tetris_map, const int line);
int can_tetramino_move_down(const tetramino *const tetramino, const tetris_map *const tetris_map);
int can_tetramino_move_left(const tetramino *const tetramino, const tetris_map *const tetris_map);
int can_tetramino_move_right(const tetramino *const tetramino, const tetris_map *const tetris_map);
int tetramino_move_down(tetramino *const tetramino, tetris_map *const tetris_map);
int tetramino_move_left(tetramino *const tetramino, const tetris_map *const tetris_map);
int tetramino_move_right(tetramino *const tetramino, const tetris_map *const tetris_map);
void tetramino_init(tetramino *const tetramino, const tetris_map *const tetris_map);
void tetramino_destroy(tetramino *const tetramino);
void tetris_map_init(tetris_map *const tetris_map, const Uint32 height, const Uint32 width);
void tetris_map_destroy(tetris_map *const tetris_map);

#endif