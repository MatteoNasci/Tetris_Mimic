#include "aiv_unit_test.h"
#include "tetris.h"
#include "gfx.h"
#include "utils.h"

#define TETRAMINO_SETUP(name, tetris_map_name) \
	tetramino name;                            \
	tetramino_init(&name, &tetris_map_name);
#define TETRAMINI_SETUP(name, tetris_map_name, type_value) \
	tetramini name;                                        \
	tetramini_init(&name, &tetris_map_name, type_value);
#define TETRIS_MAP_SETUP(name, h, w) \
	tetris_map name;                 \
	tetris_map_init(&name, h, w);
#define TETRIS_SETUP(name_tetramino, name_tetris_map, h, w) \
	TETRIS_MAP_SETUP(name_tetris_map, h, w)                 \
	TETRAMINO_SETUP(name_tetramino, name_tetris_map)
#define FULL_TETRIS_SETUP(name_tetramini, type_value, name_tetris_map, h, w) \
	TETRIS_MAP_SETUP(name_tetris_map, h, w)                                  \
	TETRAMINI_SETUP(name_tetramini, name_tetris_map, type_value)
#define TETRAMINO_DESTROY(name) tetramino_destroy(&name);
#define TETRAMINI_DESTROY(name) tetramini_destroy(&name);
#define TETRIS_MAP_DESTROY(name) tetris_map_destroy(&name);
#define TETRIS_DESTROY(name_tetramino, name_tetris_map) \
	TETRAMINO_DESTROY(name_tetramino)                   \
	TETRIS_MAP_DESTROY(name_tetris_map)
#define FULL_TETRIS_DESTROY(name_tetramini, name_tetris_map) \
	TETRAMINI_DESTROY(name_tetramini)                        \
	TETRIS_MAP_DESTROY(name_tetris_map)

TEST(precise_rand)
{
	register int i;
	for (i = 0; i < 1000; i++)
	{
		int result = precise_rand_lim(i);
		ASSERT_THAT(IS_INCLUDED_INCLUSIVE(result, 0, i));
	}
}
TEST(quick_rand_range)
{
	register int i;
	register int j;
	for (i = 1; i < 1000; i++)
	{
		j = precise_rand_lim(i - 1);
		int result = quick_rand_range(j, i);
		ASSERT_THAT(IS_INCLUDED_INCLUSIVE(result, j, i));
	}
}
TEST(tetramino_init)
{
	int i;
	for (i = 0; i < 1000; i++)
	{
		TETRIS_SETUP(tetramino, tetris_map, i + 1, i + 1);
		ASSERT_THAT(tetramino.y == TETRAMINO_INITIAL_HEIGHT);
		int value = MIN(tetramino.x, tetris_map.width - 1);
		ASSERT_THAT(value == tetramino.x);
		value = MAX(tetramino.x, 0);
		ASSERT_THAT(value == tetramino.x);
		TETRIS_DESTROY(tetramino, tetris_map);
	}
}
TEST(is_line_filled)
{
	TETRIS_SETUP(tetramino, tetris_map, 3, 3);
	tetris_map.cells[0] = 1;
	tetris_map.cells[1] = 1;
	tetris_map.cells[2] = 0;
	tetris_map.cells[3] = 1;
	tetris_map.cells[4] = 1;
	tetris_map.cells[5] = 0;
	tetris_map.cells[6] = 1;
	tetris_map.cells[7] = 1;
	tetris_map.cells[8] = 1;
	ASSERT_THAT(is_line_filled(&tetris_map, 1) == TETRIS_MAP_LINE_NOT_FILLED);
	ASSERT_THAT(is_line_filled(&tetris_map, 2) == TETRIS_MAP_LINE_NOT_FILLED);
	ASSERT_THAT(is_line_filled(&tetris_map, 3) == TETRIS_MAP_LINE_FILLED);
	tetris_map.cells[2] = 1;
	ASSERT_THAT(is_line_filled(&tetris_map, 1) == TETRIS_MAP_LINE_FILLED);
	TETRIS_DESTROY(tetramino, tetris_map);
}
//int tetramini_rotate(tetramini *const tetramini, const tetris_map *const tetris_map);
TEST(tetramini_rotate)
{
	FULL_TETRIS_SETUP(tetramini, 1, tetris_map, 100, 200);
	tetramini.arr_tetramini[0].x = 5;
	tetramini.arr_tetramini[1].x = 5;
	tetramini.arr_tetramini[2].x = 5;
	tetramini.arr_tetramini[3].x = 5;
	ASSERT_THAT(tetramini_rotate(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_rotate(&tetramini, &tetris_map) == TETRAMINO_OK);
	tetramini.arr_tetramini[0].y += 5;
	tetramini.arr_tetramini[1].y += 5;
	tetramini.arr_tetramini[2].y += 5;
	tetramini.arr_tetramini[3].y += 5;
	ASSERT_THAT(tetramini.rot_state == 2);
	ASSERT_THAT(tetramini.arr_tetramini[0].x == 5);
	ASSERT_THAT(tetramini.arr_tetramini[0].y == 4);
	ASSERT_THAT(tetramini.arr_tetramini[1].x == 5);
	ASSERT_THAT(tetramini.arr_tetramini[1].y == 3);
	ASSERT_THAT(tetramini.arr_tetramini[2].x == 5);
	ASSERT_THAT(tetramini.arr_tetramini[2].y == 2);
	ASSERT_THAT(tetramini.arr_tetramini[3].x == 5);
	ASSERT_THAT(tetramini.arr_tetramini[3].y == 1);
	ASSERT_THAT(tetramini_rotate(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini.rot_state == 3);
	ASSERT_THAT(tetramini.arr_tetramini[0].x == 5);
	ASSERT_THAT(tetramini.arr_tetramini[0].y == 4);
	ASSERT_THAT(tetramini.arr_tetramini[1].x == 6);
	ASSERT_THAT(tetramini.arr_tetramini[1].y == 4);
	ASSERT_THAT(tetramini.arr_tetramini[2].x == 7);
	ASSERT_THAT(tetramini.arr_tetramini[2].y == 4);
	ASSERT_THAT(tetramini.arr_tetramini[3].x == 8);
	ASSERT_THAT(tetramini.arr_tetramini[3].y == 4);
	ASSERT_THAT(tetramini_rotate(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini.rot_state == 0);
	ASSERT_THAT(tetramini.arr_tetramini[0].x == 5);
	ASSERT_THAT(tetramini.arr_tetramini[0].y == 4);
	ASSERT_THAT(tetramini.arr_tetramini[1].x == 5);
	ASSERT_THAT(tetramini.arr_tetramini[1].y == 3);
	ASSERT_THAT(tetramini.arr_tetramini[2].x == 5);
	ASSERT_THAT(tetramini.arr_tetramini[2].y == 2);
	ASSERT_THAT(tetramini.arr_tetramini[3].x == 5);
	ASSERT_THAT(tetramini.arr_tetramini[3].y == 1);
	ASSERT_THAT(tetramini_rotate(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini.rot_state == 1);
	ASSERT_THAT(tetramini.arr_tetramini[0].x == 5);
	ASSERT_THAT(tetramini.arr_tetramini[0].y == 4);
	ASSERT_THAT(tetramini.arr_tetramini[1].x == 6);
	ASSERT_THAT(tetramini.arr_tetramini[1].y == 4);
	ASSERT_THAT(tetramini.arr_tetramini[2].x == 7);
	ASSERT_THAT(tetramini.arr_tetramini[2].y == 4);
	ASSERT_THAT(tetramini.arr_tetramini[3].x == 8);
	ASSERT_THAT(tetramini.arr_tetramini[3].y == 4);
	ASSERT_THAT(tetramini_rotate(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini.rot_state == 2);
	ASSERT_THAT(tetramini.arr_tetramini[0].x == 5);
	ASSERT_THAT(tetramini.arr_tetramini[0].y == 4);
	ASSERT_THAT(tetramini.arr_tetramini[1].x == 5);
	ASSERT_THAT(tetramini.arr_tetramini[1].y == 3);
	ASSERT_THAT(tetramini.arr_tetramini[2].x == 5);
	ASSERT_THAT(tetramini.arr_tetramini[2].y == 2);
	ASSERT_THAT(tetramini.arr_tetramini[3].x == 5);
	ASSERT_THAT(tetramini.arr_tetramini[3].y == 1);
	FULL_TETRIS_DESTROY(tetramini, tetris_map);
}
//int tetramini_rotate(tetramini *const tetramini, const tetris_map *const tetris_map);
TEST(tetramini_rotate_to_end)
{
	FULL_TETRIS_SETUP(tetramini, 1, tetris_map, 10, 20);
	tetramini.arr_tetramini[0].x = 5;
	tetramini.arr_tetramini[0].y = 3;
	tetramini.arr_tetramini[1].x = 5;
	tetramini.arr_tetramini[1].y = 2;
	tetramini.arr_tetramini[2].x = 5;
	tetramini.arr_tetramini[2].y = 1;
	tetramini.arr_tetramini[3].x = 5;
	tetramini.arr_tetramini[3].y = 0;
	tetris_map.cells[66] = 1;
	ASSERT_THAT(tetramini_rotate(&tetramini, &tetris_map) == TETRAMINO_BLOCKED);
	ASSERT_THAT(tetramini.rot_state == 0);
	tetris_map.cells[66] = 0;
	ASSERT_THAT(tetramini_rotate(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini.rot_state == 1);
	tetris_map.cells[5] = 1;
	ASSERT_THAT(tetramini_rotate(&tetramini, &tetris_map) == TETRAMINO_BLOCKED);
	ASSERT_THAT(tetramini.rot_state == 1);
	FULL_TETRIS_DESTROY(tetramini, tetris_map);
}
//int tetramini_rotate(tetramini *const tetramini, const tetris_map *const tetris_map);
TEST(tetramini_rotate_to_border)
{
	FULL_TETRIS_SETUP(tetramini, 1, tetris_map, 10, 20);
	tetramini.arr_tetramini[0].x = 17;
	tetramini.arr_tetramini[1].x = 17;
	tetramini.arr_tetramini[2].x = 17;
	tetramini.arr_tetramini[3].x = 17;
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_rotate(&tetramini, &tetris_map) == TETRAMINO_BLOCKED);
	ASSERT_THAT(tetramini.rot_state == 0);
	FULL_TETRIS_DESTROY(tetramini, tetris_map);
}
//int can_tetramini_move_down(const tetramini *const tetramini, const tetris_map *const tetris_map);
TEST(can_tetramini_move_down)
{
	FULL_TETRIS_SETUP(tetramini, 1, tetris_map, 10, 20);
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	tetramini.arr_tetramini[0].y++;
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	tetramini.arr_tetramini[0].y++;
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	tetramini.arr_tetramini[0].y++;
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	tetramini.arr_tetramini[0].y++;
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	tetramini.arr_tetramini[0].y++;
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	tetramini.arr_tetramini[0].y++;
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	tetramini.arr_tetramini[0].y++;
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	tetramini.arr_tetramini[0].y++;
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	tetramini.arr_tetramini[0].y++;
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	tetramini.arr_tetramini[0].y++;
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_STOPPED);

	FULL_TETRIS_DESTROY(tetramini, tetris_map);
}
//int can_tetramini_move_left(const tetramini *const tetramini, const tetris_map *const tetris_map);
TEST(can_tetramini_move_left)
{
	FULL_TETRIS_SETUP(tetramini, 1, tetris_map, 10, 20);
	tetramini.arr_tetramini[0].x = 2;
	tetramini.arr_tetramini[1].x = 2;
	tetramini.arr_tetramini[2].x = 2;
	tetramini.arr_tetramini[3].x = 2;
	ASSERT_THAT(can_tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);
	tetramini.arr_tetramini[0].x = 1;
	tetramini.arr_tetramini[1].x = 1;
	tetramini.arr_tetramini[2].x = 1;
	tetramini.arr_tetramini[3].x = 1;
	ASSERT_THAT(can_tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);
	tetramini.arr_tetramini[0].x = 0;
	tetramini.arr_tetramini[1].x = 1;
	tetramini.arr_tetramini[2].x = 1;
	tetramini.arr_tetramini[3].x = 1;
	ASSERT_THAT(can_tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_BLOCKED);
	ASSERT_THAT(tetramini.arr_tetramini[0].x == 0);
	ASSERT_THAT(tetramini.arr_tetramini[1].x == 1);
	ASSERT_THAT(tetramini.arr_tetramini[2].x == 1);
	ASSERT_THAT(tetramini.arr_tetramini[3].x == 1);

	FULL_TETRIS_DESTROY(tetramini, tetris_map);
}
//int can_tetramini_move_right(const tetramini *const tetramini, const tetris_map *const tetris_map);
TEST(can_tetramini_move_right)
{
	FULL_TETRIS_SETUP(tetramini, 1, tetris_map, 10, 20);
	tetramini.arr_tetramini[0].x = 17;
	tetramini.arr_tetramini[1].x = 16;
	tetramini.arr_tetramini[2].x = 15;
	tetramini.arr_tetramini[3].x = 14;
	ASSERT_THAT(can_tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);
	tetramini.arr_tetramini[0].x = 18;
	tetramini.arr_tetramini[1].x = 1;
	tetramini.arr_tetramini[2].x = 1;
	tetramini.arr_tetramini[3].x = 1;
	ASSERT_THAT(can_tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);
	tetramini.arr_tetramini[0].x = 0;
	tetramini.arr_tetramini[1].x = 1;
	tetramini.arr_tetramini[2].x = 1;
	tetramini.arr_tetramini[3].x = 19;
	ASSERT_THAT(can_tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_BLOCKED);
	ASSERT_THAT(tetramini.arr_tetramini[0].x == 0);
	ASSERT_THAT(tetramini.arr_tetramini[1].x == 1);
	ASSERT_THAT(tetramini.arr_tetramini[2].x == 1);
	ASSERT_THAT(tetramini.arr_tetramini[3].x == 19);

	FULL_TETRIS_DESTROY(tetramini, tetris_map);
}
//int can_tetramini_move_down(const tetramini *const tetramini, const tetris_map *const tetris_map);
TEST(can_tetramini_move_down_till_end)
{
	FULL_TETRIS_SETUP(tetramini, 1, tetris_map, 10, 20);
	tetramini.arr_tetramini[0].x = 0;
	tetramini.arr_tetramini[1].x = 0;
	tetramini.arr_tetramini[2].x = 0;
	tetramini.arr_tetramini[3].x = 0;
	tetris_map.cells[160] = 1;
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);

	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);

	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_STOPPED);
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_STOPPED);
	FULL_TETRIS_DESTROY(tetramini, tetris_map);
}
//int can_tetramini_move_left(const tetramini *const tetramini, const tetris_map *const tetris_map);
TEST(can_tetramini_move_left_till_end)
{
	FULL_TETRIS_SETUP(tetramini, 1, tetris_map, 10, 20);
	tetramini.arr_tetramini[0].x = 10;
	tetramini.arr_tetramini[1].x = 10;
	tetramini.arr_tetramini[2].x = 10;
	tetramini.arr_tetramini[3].x = 10;
	tetramini.arr_tetramini[0].y = 0;
	tetramini.arr_tetramini[1].y = 1;
	tetramini.arr_tetramini[2].y = 2;
	tetramini.arr_tetramini[3].y = 3;
	tetris_map.cells[1] = 1;
	ASSERT_THAT(can_tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);

	ASSERT_THAT(can_tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);

	ASSERT_THAT(can_tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_BLOCKED);
	ASSERT_THAT(tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_BLOCKED);
	FULL_TETRIS_DESTROY(tetramini, tetris_map);
}
//int can_tetramini_move_right(const tetramini *const tetramini, const tetris_map *const tetris_map);
TEST(can_tetramini_move_right_till_end)
{
	FULL_TETRIS_SETUP(tetramini, 1, tetris_map, 10, 20);
	tetramini.arr_tetramini[0].x = 14;
	tetramini.arr_tetramini[1].x = 15;
	tetramini.arr_tetramini[2].x = 15;
	tetramini.arr_tetramini[3].x = 15;
	tetramini.arr_tetramini[0].y = 0;
	tetramini.arr_tetramini[1].y = 1;
	tetramini.arr_tetramini[2].y = 2;
	tetramini.arr_tetramini[3].y = 3;
	tetris_map.cells[17] = 1;
	ASSERT_THAT(can_tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);

	ASSERT_THAT(can_tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_BLOCKED);
	ASSERT_THAT(tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_BLOCKED);
	FULL_TETRIS_DESTROY(tetramini, tetris_map);
}
//int can_tetramini_move_down(const tetramini *const tetramini, const tetris_map *const tetris_map);
TEST(can_tetramini_move_down_till_border)
{
	FULL_TETRIS_SETUP(tetramini, 1, tetris_map, 10, 20);
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);

	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);

	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_STOPPED);
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_STOPPED);
	FULL_TETRIS_DESTROY(tetramini, tetris_map);
}
//int can_tetramini_move_left(const tetramini *const tetramini, const tetris_map *const tetris_map);
TEST(can_tetramini_move_left_till_border)
{
	FULL_TETRIS_SETUP(tetramini, 1, tetris_map, 10, 20);
	tetramini.arr_tetramini[0].x = 2;
	tetramini.arr_tetramini[1].x = 2;
	tetramini.arr_tetramini[2].x = 2;
	tetramini.arr_tetramini[3].x = 2;
	tetramini.arr_tetramini[0].y = 0;
	tetramini.arr_tetramini[1].y = 1;
	tetramini.arr_tetramini[2].y = 2;
	tetramini.arr_tetramini[3].y = 3;
	ASSERT_THAT(can_tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);

	ASSERT_THAT(can_tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_BLOCKED);
	ASSERT_THAT(tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_BLOCKED);
	FULL_TETRIS_DESTROY(tetramini, tetris_map);
}
//int can_tetramini_move_right(const tetramini *const tetramini, const tetris_map *const tetris_map);
TEST(can_tetramini_move_right_till_border)
{
	FULL_TETRIS_SETUP(tetramini, 1, tetris_map, 10, 20);
	tetramini.arr_tetramini[0].x = 10;
	tetramini.arr_tetramini[1].x = 10;
	tetramini.arr_tetramini[2].x = 10;
	tetramini.arr_tetramini[3].x = 10;
	tetramini.arr_tetramini[0].y = 0;
	tetramini.arr_tetramini[1].y = 1;
	tetramini.arr_tetramini[2].y = 2;
	tetramini.arr_tetramini[3].y = 3;
	ASSERT_THAT(can_tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);

	ASSERT_THAT(can_tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);

	ASSERT_THAT(can_tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(can_tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_BLOCKED);
	ASSERT_THAT(tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_BLOCKED);
	FULL_TETRIS_DESTROY(tetramini, tetris_map);
}
//int tetramini_move_down(tetramini *const tetramini, tetris_map *const tetris_map);
TEST(tetramini_move_down)
{
	FULL_TETRIS_SETUP(tetramini, 1, tetris_map, 10, 20);
	tetramini.arr_tetramini[0].x = 10;
	tetramini.arr_tetramini[1].x = 10;
	tetramini.arr_tetramini[2].x = 10;
	tetramini.arr_tetramini[3].x = 10;
	tetramini.arr_tetramini[0].y = 0;
	tetramini.arr_tetramini[1].y = 1;
	tetramini.arr_tetramini[2].y = 2;
	tetramini.arr_tetramini[3].y = 7;
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini.arr_tetramini[0].y == 1);
	ASSERT_THAT(tetramini.arr_tetramini[1].y == 2);
	ASSERT_THAT(tetramini.arr_tetramini[2].y == 3);
	ASSERT_THAT(tetramini.arr_tetramini[3].y == 8);
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini.arr_tetramini[0].y == 2);
	ASSERT_THAT(tetramini.arr_tetramini[1].y == 3);
	ASSERT_THAT(tetramini.arr_tetramini[2].y == 4);
	ASSERT_THAT(tetramini.arr_tetramini[3].y == 9);

	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_STOPPED);

	FULL_TETRIS_DESTROY(tetramini, tetris_map);
}
//int tetramini_move_left(tetramini *const tetramini, const tetris_map *const tetris_map);
TEST(tetramini_move_left)
{
	FULL_TETRIS_SETUP(tetramini, 1, tetris_map, 10, 20);
	tetramini.arr_tetramini[0].x = 10;
	tetramini.arr_tetramini[1].x = 10;
	tetramini.arr_tetramini[2].x = 10;
	tetramini.arr_tetramini[3].x = 10;
	tetramini.arr_tetramini[0].y = 0;
	tetramini.arr_tetramini[1].y = 1;
	tetramini.arr_tetramini[2].y = 2;
	tetramini.arr_tetramini[3].y = 7;
	ASSERT_THAT(tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini.arr_tetramini[0].x == 9);
	ASSERT_THAT(tetramini.arr_tetramini[1].x == 9);
	ASSERT_THAT(tetramini.arr_tetramini[2].x == 9);
	ASSERT_THAT(tetramini.arr_tetramini[3].x == 9);
	ASSERT_THAT(tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini.arr_tetramini[0].x == 8);
	ASSERT_THAT(tetramini.arr_tetramini[1].x == 8);
	ASSERT_THAT(tetramini.arr_tetramini[2].x == 8);
	ASSERT_THAT(tetramini.arr_tetramini[3].x == 8);

	ASSERT_THAT(tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);

	FULL_TETRIS_DESTROY(tetramini, tetris_map);
}
//int tetramini_move_right(tetramini *const tetramini, const tetris_map *const tetris_map);
TEST(tetramini_move_right)
{
	FULL_TETRIS_SETUP(tetramini, 1, tetris_map, 10, 20);
	tetramini.arr_tetramini[0].x = 10;
	tetramini.arr_tetramini[1].x = 10;
	tetramini.arr_tetramini[2].x = 10;
	tetramini.arr_tetramini[3].x = 10;
	tetramini.arr_tetramini[0].y = 0;
	tetramini.arr_tetramini[1].y = 1;
	tetramini.arr_tetramini[2].y = 2;
	tetramini.arr_tetramini[3].y = 7;
	ASSERT_THAT(tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini.arr_tetramini[0].x == 11);
	ASSERT_THAT(tetramini.arr_tetramini[1].x == 11);
	ASSERT_THAT(tetramini.arr_tetramini[2].x == 11);
	ASSERT_THAT(tetramini.arr_tetramini[3].x == 11);
	ASSERT_THAT(tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini.arr_tetramini[0].x == 12);
	ASSERT_THAT(tetramini.arr_tetramini[1].x == 12);
	ASSERT_THAT(tetramini.arr_tetramini[2].x == 12);
	ASSERT_THAT(tetramini.arr_tetramini[3].x == 12);

	ASSERT_THAT(tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);

	FULL_TETRIS_DESTROY(tetramini, tetris_map);
}
//int tetramini_move_down(tetramini *const tetramini, tetris_map *const tetris_map);
TEST(tetramini_move_down_till_end)
{
	FULL_TETRIS_SETUP(tetramini, 1, tetris_map, 10, 20);
	tetramini.arr_tetramini[0].x = 0;
	tetramini.arr_tetramini[1].x = 0;
	tetramini.arr_tetramini[2].x = 0;
	tetramini.arr_tetramini[3].x = 0;
	tetramini.arr_tetramini[0].y = 0;
	tetramini.arr_tetramini[1].y = 1;
	tetramini.arr_tetramini[2].y = 2;
	tetramini.arr_tetramini[3].y = 7;
	tetris_map.cells[180] = 1;
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini.arr_tetramini[0].y == 1);
	ASSERT_THAT(tetramini.arr_tetramini[1].y == 2);
	ASSERT_THAT(tetramini.arr_tetramini[2].y == 3);
	ASSERT_THAT(tetramini.arr_tetramini[3].y == 8);

	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_STOPPED);
	ASSERT_THAT(tetramini.arr_tetramini[0].y == 1);
	ASSERT_THAT(tetramini.arr_tetramini[1].y == 2);
	ASSERT_THAT(tetramini.arr_tetramini[2].y == 3);
	ASSERT_THAT(tetramini.arr_tetramini[3].y == 8);
	ASSERT_THAT(tetris_map.cells[160] == 1);
	ASSERT_THAT(tetris_map.cells[60] == 1);
	ASSERT_THAT(tetris_map.cells[40] == 1);
	ASSERT_THAT(tetris_map.cells[20] == 1);

	FULL_TETRIS_DESTROY(tetramini, tetris_map);
}
//int tetramini_move_left(tetramini *const tetramini, const tetris_map *const tetris_map);
TEST(tetramini_move_left_till_end)
{
	FULL_TETRIS_SETUP(tetramini, 1, tetris_map, 10, 20);
	tetramini.arr_tetramini[0].x = 2;
	tetramini.arr_tetramini[1].x = 2;
	tetramini.arr_tetramini[2].x = 2;
	tetramini.arr_tetramini[3].x = 2;
	tetramini.arr_tetramini[0].y = 0;
	tetramini.arr_tetramini[1].y = 1;
	tetramini.arr_tetramini[2].y = 2;
	tetramini.arr_tetramini[3].y = 3;
	tetris_map.cells[0] = 1;
	ASSERT_THAT(tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini.arr_tetramini[0].x == 1);
	ASSERT_THAT(tetramini.arr_tetramini[1].x == 1);
	ASSERT_THAT(tetramini.arr_tetramini[2].x == 1);
	ASSERT_THAT(tetramini.arr_tetramini[3].x == 1);

	ASSERT_THAT(tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_BLOCKED);
	ASSERT_THAT(tetramini.arr_tetramini[0].x == 1);
	ASSERT_THAT(tetramini.arr_tetramini[1].x == 1);
	ASSERT_THAT(tetramini.arr_tetramini[2].x == 1);
	ASSERT_THAT(tetramini.arr_tetramini[3].x == 1);
	ASSERT_THAT(tetris_map.cells[1] == 0);

	FULL_TETRIS_DESTROY(tetramini, tetris_map);
}
//int tetramini_move_right(tetramini *const tetramini, const tetris_map *const tetris_map);
TEST(tetramini_move_right_till_end)
{
	FULL_TETRIS_SETUP(tetramini, 1, tetris_map, 10, 20);
	tetramini.arr_tetramini[0].x = 17;
	tetramini.arr_tetramini[1].x = 17;
	tetramini.arr_tetramini[2].x = 17;
	tetramini.arr_tetramini[3].x = 16;
	tetramini.arr_tetramini[0].y = 0;
	tetramini.arr_tetramini[1].y = 1;
	tetramini.arr_tetramini[2].y = 2;
	tetramini.arr_tetramini[3].y = 3;
	tetris_map.cells[19] = 1;
	ASSERT_THAT(tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini.arr_tetramini[0].x == 18);
	ASSERT_THAT(tetramini.arr_tetramini[1].x == 18);
	ASSERT_THAT(tetramini.arr_tetramini[2].x == 18);
	ASSERT_THAT(tetramini.arr_tetramini[3].x == 17);

	ASSERT_THAT(tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_BLOCKED);
	ASSERT_THAT(tetramini.arr_tetramini[0].x == 18);
	ASSERT_THAT(tetramini.arr_tetramini[1].x == 18);
	ASSERT_THAT(tetramini.arr_tetramini[2].x == 18);
	ASSERT_THAT(tetramini.arr_tetramini[3].x == 17);
	ASSERT_THAT(tetris_map.cells[18] == 0);

	FULL_TETRIS_DESTROY(tetramini, tetris_map);
}
//int tetramini_move_down(tetramini *const tetramini, tetris_map *const tetris_map);
TEST(tetramini_move_down_till_border)
{
	FULL_TETRIS_SETUP(tetramini, 1, tetris_map, 10, 20);
	tetramini.arr_tetramini[0].x = 0;
	tetramini.arr_tetramini[1].x = 0;
	tetramini.arr_tetramini[2].x = 0;
	tetramini.arr_tetramini[3].x = 0;
	tetramini.arr_tetramini[0].y = 0;
	tetramini.arr_tetramini[1].y = 1;
	tetramini.arr_tetramini[2].y = 2;
	tetramini.arr_tetramini[3].y = 7;
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini.arr_tetramini[0].y == 1);
	ASSERT_THAT(tetramini.arr_tetramini[1].y == 2);
	ASSERT_THAT(tetramini.arr_tetramini[2].y == 3);
	ASSERT_THAT(tetramini.arr_tetramini[3].y == 8);
	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini.arr_tetramini[0].y == 2);
	ASSERT_THAT(tetramini.arr_tetramini[1].y == 3);
	ASSERT_THAT(tetramini.arr_tetramini[2].y == 4);
	ASSERT_THAT(tetramini.arr_tetramini[3].y == 9);

	ASSERT_THAT(tetramini_move_down(&tetramini, &tetris_map) == TETRAMINO_STOPPED);
	ASSERT_THAT(tetramini.arr_tetramini[0].y == 2);
	ASSERT_THAT(tetramini.arr_tetramini[1].y == 3);
	ASSERT_THAT(tetramini.arr_tetramini[2].y == 4);
	ASSERT_THAT(tetramini.arr_tetramini[3].y == 9);
	ASSERT_THAT(tetris_map.cells[180] == 1);
	ASSERT_THAT(tetris_map.cells[80] == 1);
	ASSERT_THAT(tetris_map.cells[60] == 1);
	ASSERT_THAT(tetris_map.cells[40] == 1);

	FULL_TETRIS_DESTROY(tetramini, tetris_map);
}
//int tetramini_move_left(tetramini *const tetramini, const tetris_map *const tetris_map);
TEST(tetramini_move_left_till_border)
{
	FULL_TETRIS_SETUP(tetramini, 1, tetris_map, 10, 20);
	tetramini.arr_tetramini[0].x = 2;
	tetramini.arr_tetramini[1].x = 2;
	tetramini.arr_tetramini[2].x = 2;
	tetramini.arr_tetramini[3].x = 2;
	tetramini.arr_tetramini[0].y = 0;
	tetramini.arr_tetramini[1].y = 1;
	tetramini.arr_tetramini[2].y = 2;
	tetramini.arr_tetramini[3].y = 3;
	ASSERT_THAT(tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini.arr_tetramini[0].x == 1);
	ASSERT_THAT(tetramini.arr_tetramini[1].x == 1);
	ASSERT_THAT(tetramini.arr_tetramini[2].x == 1);
	ASSERT_THAT(tetramini.arr_tetramini[3].x == 1);

	ASSERT_THAT(tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini.arr_tetramini[0].x == 0);
	ASSERT_THAT(tetramini.arr_tetramini[1].x == 0);
	ASSERT_THAT(tetramini.arr_tetramini[2].x == 0);
	ASSERT_THAT(tetramini.arr_tetramini[3].x == 0);

	ASSERT_THAT(tetramini_move_left(&tetramini, &tetris_map) == TETRAMINO_BLOCKED);
	ASSERT_THAT(tetramini.arr_tetramini[0].x == 0);
	ASSERT_THAT(tetramini.arr_tetramini[1].x == 0);
	ASSERT_THAT(tetramini.arr_tetramini[2].x == 0);
	ASSERT_THAT(tetramini.arr_tetramini[3].x == 0);
	ASSERT_THAT(tetris_map.cells[0] == 0);

	FULL_TETRIS_DESTROY(tetramini, tetris_map);
}
//int tetramini_move_right(tetramini *const tetramini, const tetris_map *const tetris_map);
TEST(tetramini_move_right_till_border)
{
	FULL_TETRIS_SETUP(tetramini, 1, tetris_map, 10, 20);
	tetramini.arr_tetramini[0].x = 17;
	tetramini.arr_tetramini[1].x = 17;
	tetramini.arr_tetramini[2].x = 17;
	tetramini.arr_tetramini[3].x = 16;
	tetramini.arr_tetramini[0].y = 0;
	tetramini.arr_tetramini[1].y = 1;
	tetramini.arr_tetramini[2].y = 2;
	tetramini.arr_tetramini[3].y = 3;
	ASSERT_THAT(tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini.arr_tetramini[0].x == 18);
	ASSERT_THAT(tetramini.arr_tetramini[1].x == 18);
	ASSERT_THAT(tetramini.arr_tetramini[2].x == 18);
	ASSERT_THAT(tetramini.arr_tetramini[3].x == 17);
	ASSERT_THAT(tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramini.arr_tetramini[0].x == 19);
	ASSERT_THAT(tetramini.arr_tetramini[1].x == 19);
	ASSERT_THAT(tetramini.arr_tetramini[2].x == 19);
	ASSERT_THAT(tetramini.arr_tetramini[3].x == 18);

	ASSERT_THAT(tetramini_move_right(&tetramini, &tetris_map) == TETRAMINO_BLOCKED);
	ASSERT_THAT(tetramini.arr_tetramini[0].x == 19);
	ASSERT_THAT(tetramini.arr_tetramini[1].x == 19);
	ASSERT_THAT(tetramini.arr_tetramini[2].x == 19);
	ASSERT_THAT(tetramini.arr_tetramini[3].x == 18);
	ASSERT_THAT(tetris_map.cells[19] == 0);

	FULL_TETRIS_DESTROY(tetramini, tetris_map);
}
//void tetramini_init(tetramini *const tetramini, const tetris_map *const tetris_map, int* type);
TEST(tetramini_init_0)
{
	FULL_TETRIS_SETUP(tetramini, 0, tetris_map, 5, 20);
	ASSERT_THAT(tetramini.rot_state == 0);
	ASSERT_THAT(tetramini.type == 0);
	ASSERT_THAT(IS_INCLUDED_INCLUSIVE(tetramini.arr_tetramini[0].x, 0, 19));
	ASSERT_THAT(IS_INCLUDED_INCLUSIVE(tetramini.arr_tetramini[1].x, 0, 19));
	ASSERT_THAT(IS_INCLUDED_INCLUSIVE(tetramini.arr_tetramini[2].x, 0, 19));
	ASSERT_THAT(IS_INCLUDED_INCLUSIVE(tetramini.arr_tetramini[3].x, 0, 19));
	ASSERT_THAT(tetramini.arr_tetramini[0].y == -1);
	ASSERT_THAT(tetramini.arr_tetramini[1].y == -1);
	ASSERT_THAT(tetramini.arr_tetramini[2].y == -2);
	ASSERT_THAT(tetramini.arr_tetramini[3].y == -2);
	FULL_TETRIS_DESTROY(tetramini, tetris_map);
}
TEST(tetramini_init_2)
{
	FULL_TETRIS_SETUP(tetramini, 2, tetris_map, 5, 10);
	ASSERT_THAT(tetramini.rot_state == 0);
	ASSERT_THAT(tetramini.type == 2);
	ASSERT_THAT(IS_INCLUDED_INCLUSIVE(tetramini.arr_tetramini[0].x, 0, 9));
	ASSERT_THAT(IS_INCLUDED_INCLUSIVE(tetramini.arr_tetramini[1].x, 0, 9));
	ASSERT_THAT(IS_INCLUDED_INCLUSIVE(tetramini.arr_tetramini[2].x, 0, 9));
	ASSERT_THAT(IS_INCLUDED_INCLUSIVE(tetramini.arr_tetramini[3].x, 0, 9));
	ASSERT_THAT(tetramini.arr_tetramini[0].y == -1);
	ASSERT_THAT(tetramini.arr_tetramini[1].y == -2);
	ASSERT_THAT(tetramini.arr_tetramini[2].y == -3);
	ASSERT_THAT(tetramini.arr_tetramini[3].y == -3);
	FULL_TETRIS_DESTROY(tetramini, tetris_map);
}
TEST(tetramini_init_6)
{
	FULL_TETRIS_SETUP(tetramini, TETRAMINI_NUM - 1, tetris_map, 5, 8);
	ASSERT_THAT(tetramini.rot_state == 0);
	ASSERT_THAT(tetramini.type == TETRAMINI_NUM - 1);
	ASSERT_THAT(IS_INCLUDED_INCLUSIVE(tetramini.arr_tetramini[0].x, 0, 7));
	ASSERT_THAT(IS_INCLUDED_INCLUSIVE(tetramini.arr_tetramini[1].x, 0, 7));
	ASSERT_THAT(IS_INCLUDED_INCLUSIVE(tetramini.arr_tetramini[2].x, 0, 7));
	ASSERT_THAT(IS_INCLUDED_INCLUSIVE(tetramini.arr_tetramini[3].x, 0, 7));
	ASSERT_THAT(tetramini.arr_tetramini[0].y == -1);
	ASSERT_THAT(tetramini.arr_tetramini[1].y == -2);
	ASSERT_THAT(tetramini.arr_tetramini[2].y == -2);
	ASSERT_THAT(tetramini.arr_tetramini[3].y == -3);
	FULL_TETRIS_DESTROY(tetramini, tetris_map);
}
TEST(tetramini_init_random)
{
	FULL_TETRIS_SETUP(tetramini, -5, tetris_map, 5, 20);
	ASSERT_THAT(IS_INCLUDED_INCLUSIVE(tetramini.type, 0, TETRAMINI_NUM - 1));
	ASSERT_THAT(tetramini.rot_state == 0);
	ASSERT_THAT(IS_INCLUDED_INCLUSIVE(tetramini.arr_tetramini[0].x, 0, 19));
	ASSERT_THAT(IS_INCLUDED_INCLUSIVE(tetramini.arr_tetramini[1].x, 0, 19));
	ASSERT_THAT(IS_INCLUDED_INCLUSIVE(tetramini.arr_tetramini[2].x, 0, 19));
	ASSERT_THAT(IS_INCLUDED_INCLUSIVE(tetramini.arr_tetramini[3].x, 0, 19));
	ASSERT_THAT(IS_INCLUDED_INCLUSIVE(tetramini.arr_tetramini[0].y, -1, 0));
	ASSERT_THAT(IS_INCLUDED_INCLUSIVE(tetramini.arr_tetramini[1].y, -2, 0));
	ASSERT_THAT(IS_INCLUDED_INCLUSIVE(tetramini.arr_tetramini[2].y, -3, 0));
	ASSERT_THAT(IS_INCLUDED_INCLUSIVE(tetramini.arr_tetramini[3].y, -4, -1));
	FULL_TETRIS_DESTROY(tetramini, tetris_map);
}
//void tetramini_destroy(tetramini *const tetramini);
TEST(tetramini_destroy)
{
	FULL_TETRIS_SETUP(tetramini, TETRAMINI_NUM - 1, tetris_map, 5, 20);
	FULL_TETRIS_DESTROY(tetramini, tetris_map);
	ASSERT_THAT(tetramini.rot_state == 0);
	ASSERT_THAT(tetramini.type == 0);
	register int i;
	for (i = 0; i < TETRAMINI_SIZE; i++)
	{
		ASSERT_THAT(tetramini.arr_tetramini[i].x == 0);
		ASSERT_THAT(tetramini.arr_tetramini[i].y == 0);
	}
}
//void tetramini_copy(tetramini *const to_fill, const tetramini *const to_copy);
TEST(tetramini_copy)
{
	FULL_TETRIS_SETUP(tetramini_one, TETRAMINI_NUM - 1, tetris_map, 5, 20);
	tetramini_one.rot_state = 1;
	TETRAMINI_SETUP(tetramini_two, tetris_map, 1);
	tetramini_copy(&tetramini_one, &tetramini_two);
	ASSERT_THAT(tetramini_one.rot_state == 0);
	ASSERT_THAT(tetramini_one.type == 1);
	ASSERT_THAT(tetramini_one.arr_tetramini[0].y == -1);
	ASSERT_THAT(tetramini_one.arr_tetramini[1].y == -2);
	ASSERT_THAT(tetramini_one.arr_tetramini[2].y == -3);
	ASSERT_THAT(tetramini_one.arr_tetramini[3].y == -4);
	FULL_TETRIS_DESTROY(tetramini_one, tetris_map);
	TETRAMINI_DESTROY(tetramini_two)
}
//int min_value(int len, ...);
TEST(min_value)
{
	ASSERT_THAT(min_value(8, 55, -5, 19441, -99852, 0, 5, 6, -99853) == -99853);
	ASSERT_THAT(min_value(8, 52435, 0, 194421, 9933852, 0, 5, 6, 99853) == 0);
}
//int max_value(int len, ...);
TEST(max_value)
{
	ASSERT_THAT(max_value(8, 55, -5, 19441, -99852, 0, 5, 6, -99853) == 19441);
	ASSERT_THAT(max_value(8, 52435, 0, 194421, 9933852, 0, 5, 6, 99853) == 9933852);
}
//tetramini_data* get_type_data(const int tetramini_type);
TEST(get_type_data)
{
	tetramini_data *selected = get_type_data(1);
	ASSERT_THAT(selected != NULL);
	ASSERT_THAT(selected->valid_rot == TETRAMINI_ROTATION);
	ASSERT_THAT(selected->color_r == 0);
	ASSERT_THAT(selected->color_g == 255);
	ASSERT_THAT(selected->color_b == 0);
}
//int tetramini_draw(const tetramini *const tetramini, SDL_Renderer *const renderer, const int sqare_size, const color_rgba_8 *const color);
TEST(tetramini_draw)
{
	ASSERT_THAT(0);
}
//int tetramino_draw(const tetramino *const tetramino, SDL_Renderer *const renderer, const int sqare_size, const color_rgba_8 *const color);
TEST(tetramino_draw)
{
	ASSERT_THAT(0);
}
//int tetramino_draw(const tetramino *const tetramino, SDL_Renderer *const renderer, const int sqare_size, const color_rgba_8 *const color);
TEST(tetramini_draw_offset)
{
	ASSERT_THAT(0);
}
//void set_color_rgba_8_from_tetramini_type(color_rgba_8 *const to_fill, const int type);
TEST(set_color_rgba_8_from_tetramini_type)
{
	color_rgba_8 color;
	memset(&color, 0, sizeof(color_rgba_8));
	ASSERT_THAT(color.r == 0);
	ASSERT_THAT(color.g == 0);
	ASSERT_THAT(color.b == 0);
	ASSERT_THAT(color.a == 0);
	set_color_rgba_8_from_tetramini_type(&color, 1);
	ASSERT_THAT(color.r == 0);
	ASSERT_THAT(color.g == 255);
	ASSERT_THAT(color.b == 0);
	ASSERT_THAT(color.a == 255);
}
//int tetris_map_draw(const tetris_map *const tetramino, SDL_Renderer *const renderer, const int sqare_size, const color_rgba_8 *const color);
TEST(tetris_map_draw)
{
	ASSERT_THAT(0);
}
//int _tetris_draw_internal(SDL_Renderer *const renderer, const SDL_Rect *const rect, const color_rgba_8 *const color);
TEST(_tetris_draw_internal)
{
	ASSERT_THAT(0);
}
TEST(color_rgba_8_copy)
{
	color_rgba_8 color_one;
	set_color_rgba_8(&color_one, 10, 15, 33, 66);
	color_rgba_8 color_two;
	set_color_rgba_8(&color_two, 44, 2, 2, 126);
	void *p1 = &color_one;
	void *p2 = &color_two;
	ASSERT_THAT(&color_two == p2);
	ASSERT_THAT(&color_one == p1);
	ASSERT_THAT(&color_one != &color_two);
	color_rgba_8_copy(&color_one, &color_two);
	ASSERT_THAT(color_one.r == 44);
	ASSERT_THAT(color_one.g == 2);
	ASSERT_THAT(color_one.b == 2);
	ASSERT_THAT(color_one.a == 126);
	ASSERT_THAT(color_two.r == 44);
	ASSERT_THAT(color_two.g == 2);
	ASSERT_THAT(color_two.b == 2);
	ASSERT_THAT(color_two.a == 126);
	ASSERT_THAT(&color_two == p2);
	ASSERT_THAT(&color_one == p1);
	ASSERT_THAT(&color_one != &color_two);
}
TEST(set_color_rgba_8)
{
	color_rgba_8 color;
	memset(&color, 0, sizeof(color_rgba_8));
	ASSERT_THAT(color.r == 0);
	ASSERT_THAT(color.g == 0);
	ASSERT_THAT(color.b == 0);
	ASSERT_THAT(color.a == 0);
	set_color_rgba_8(&color, 10, 15, 33, 66);
	ASSERT_THAT(color.r == 10);
	ASSERT_THAT(color.g == 15);
	ASSERT_THAT(color.b == 33);
	ASSERT_THAT(color.a == 66);
}
TEST(set_random_color_rgba_8)
{
	color_rgba_8 color;
	memset(&color, 0, sizeof(color_rgba_8));
	ASSERT_THAT(color.r == 0);
	ASSERT_THAT(color.g == 0);
	ASSERT_THAT(color.b == 0);
	ASSERT_THAT(color.a == 0);
	int i;
	for (i = 0; i < 1000; i++)
	{
		Uint8 min = precise_rand_lim(255);
		Uint8 max = quick_rand_range(min, 255);
		set_random_color_rgba_8(&color, min, max, 22);
		ASSERT_THAT(IS_INCLUDED_INCLUSIVE(color.r, min, max));
		ASSERT_THAT(IS_INCLUDED_INCLUSIVE(color.g, min, max));
		ASSERT_THAT(IS_INCLUDED_INCLUSIVE(color.b, min, max));
		ASSERT_THAT(color.a == 22);
	}
}
TEST(copy_tetramino)
{
	TETRIS_SETUP(tetramino_first, tetris_map, 3, 3);
	TETRAMINO_SETUP(tetramino_other, tetris_map);
	void *p1 = &tetramino_first;
	void *p2 = &tetramino_other;
	tetramino_first.x = 2;
	tetramino_first.y = -3;
	tetramino_other.x = -12;
	tetramino_other.y = 222;
	ASSERT_THAT(&tetramino_other == p2);
	ASSERT_THAT(&tetramino_first == p1);
	ASSERT_THAT(&tetramino_other != &tetramino_first);
	tetramino_copy(&tetramino_first, &tetramino_other);
	ASSERT_THAT(tetramino_first.x == -12);
	ASSERT_THAT(tetramino_first.y == 222);
	ASSERT_THAT(tetramino_other.x == -12);
	ASSERT_THAT(tetramino_other.y == 222);
	ASSERT_THAT(&tetramino_other == p2);
	ASSERT_THAT(&tetramino_first == p1);
	ASSERT_THAT(&tetramino_other != &tetramino_first);
	TETRIS_DESTROY(tetramino_first, tetris_map);
	TETRAMINO_DESTROY(tetramino_other);
}
TEST(is_line_filled_custom_line)
{
	TETRIS_SETUP(tetramino, tetris_map, 3, 3);
	tetris_map.cells[0] = 1;
	tetris_map.cells[1] = 1;
	tetris_map.cells[2] = 0;
	tetris_map.cells[3] = 1;
	tetris_map.cells[4] = 1;
	tetris_map.cells[5] = 1;
	tetris_map.cells[6] = 0;
	tetris_map.cells[7] = 0;
	tetris_map.cells[8] = 0;
	ASSERT_THAT(is_line_filled(&tetris_map, 1) == TETRIS_MAP_LINE_NOT_FILLED);
	ASSERT_THAT(is_line_filled(&tetris_map, 2) == TETRIS_MAP_LINE_FILLED);
	ASSERT_THAT(is_line_filled(&tetris_map, 3) == TETRIS_MAP_LINE_NOT_FILLED);
	TETRIS_DESTROY(tetramino, tetris_map);
}
TEST(free_filled_lines)
{
	TETRIS_SETUP(tetramino, tetris_map, 5, 3);
	tetris_map.cells[0] = 1;
	tetris_map.cells[1] = 0;
	tetris_map.cells[2] = 1;
	tetris_map.cells[3] = 1;
	tetris_map.cells[4] = 0;
	tetris_map.cells[5] = 0;
	tetris_map.cells[6] = 1;
	tetris_map.cells[7] = 1;
	tetris_map.cells[8] = 1;
	tetris_map.cells[9] = 1;
	tetris_map.cells[10] = 1;
	tetris_map.cells[11] = 1;
	tetris_map.cells[12] = 0;
	tetris_map.cells[13] = 1;
	tetris_map.cells[14] = 0;
	ASSERT_THAT(is_line_filled(&tetris_map, 1) == TETRIS_MAP_LINE_NOT_FILLED);
	ASSERT_THAT(is_line_filled(&tetris_map, 2) == TETRIS_MAP_LINE_NOT_FILLED);
	ASSERT_THAT(is_line_filled(&tetris_map, 3) == TETRIS_MAP_LINE_FILLED);
	ASSERT_THAT(is_line_filled(&tetris_map, 4) == TETRIS_MAP_LINE_FILLED);
	ASSERT_THAT(is_line_filled(&tetris_map, 5) == TETRIS_MAP_LINE_NOT_FILLED);
	ASSERT_THAT(free_filled_lines(&tetris_map, 1, 5) == 2);
	ASSERT_THAT(is_line_filled(&tetris_map, 1) == TETRIS_MAP_LINE_NOT_FILLED);
	ASSERT_THAT(is_line_filled(&tetris_map, 2) == TETRIS_MAP_LINE_NOT_FILLED);
	ASSERT_THAT(is_line_filled(&tetris_map, 3) == TETRIS_MAP_LINE_NOT_FILLED);
	ASSERT_THAT(is_line_filled(&tetris_map, 4) == TETRIS_MAP_LINE_NOT_FILLED);
	ASSERT_THAT(is_line_filled(&tetris_map, 5) == TETRIS_MAP_LINE_NOT_FILLED);
	ASSERT_THAT(tetris_map.cells[0] == 0);
	ASSERT_THAT(tetris_map.cells[1] == 0);
	ASSERT_THAT(tetris_map.cells[2] == 0);
	ASSERT_THAT(tetris_map.cells[3] == 0);
	ASSERT_THAT(tetris_map.cells[4] == 0);
	ASSERT_THAT(tetris_map.cells[5] == 0);
	ASSERT_THAT(tetris_map.cells[6] == 1);
	ASSERT_THAT(tetris_map.cells[7] == 0);
	ASSERT_THAT(tetris_map.cells[8] == 1);
	ASSERT_THAT(tetris_map.cells[9] == 1);
	ASSERT_THAT(tetris_map.cells[10] == 0);
	ASSERT_THAT(tetris_map.cells[11] == 0);
	ASSERT_THAT(tetris_map.cells[12] == 0);
	ASSERT_THAT(tetris_map.cells[13] == 1);
	ASSERT_THAT(tetris_map.cells[14] == 0);
	TETRIS_DESTROY(tetramino, tetris_map);
}
TEST(free_line)
{
	TETRIS_SETUP(tetramino, tetris_map, 3, 3);
	tetris_map.cells[0] = 1;
	tetris_map.cells[1] = 1;
	tetris_map.cells[2] = 1;
	tetris_map.cells[3] = 1;
	tetris_map.cells[4] = 0;
	tetris_map.cells[5] = 1;
	tetris_map.cells[6] = 0;
	tetris_map.cells[7] = 1;
	tetris_map.cells[8] = 1;
	ASSERT_THAT(is_line_filled(&tetris_map, 1) == TETRIS_MAP_LINE_FILLED);
	free_line(&tetris_map, 1);
	ASSERT_THAT(is_line_filled(&tetris_map, 1) == TETRIS_MAP_LINE_NOT_FILLED);
	ASSERT_THAT(tetris_map.cells[0] == 0);
	ASSERT_THAT(tetris_map.cells[1] == 0);
	ASSERT_THAT(tetris_map.cells[2] == 0);
	ASSERT_THAT(tetris_map.cells[3] == 1);
	ASSERT_THAT(tetris_map.cells[4] == 0);
	ASSERT_THAT(tetris_map.cells[5] == 1);
	ASSERT_THAT(tetris_map.cells[6] == 0);
	ASSERT_THAT(tetris_map.cells[7] == 1);
	ASSERT_THAT(tetris_map.cells[8] == 1);
	TETRIS_DESTROY(tetramino, tetris_map);
}
TEST(free_line_custom_value)
{
	TETRIS_SETUP(tetramino, tetris_map, 4, 3);
	tetris_map.cells[0] = 1;
	tetris_map.cells[1] = 0;
	tetris_map.cells[2] = 1;
	tetris_map.cells[3] = 1;
	tetris_map.cells[4] = 0;
	tetris_map.cells[5] = 0;
	tetris_map.cells[6] = 1;
	tetris_map.cells[7] = 1;
	tetris_map.cells[8] = 1;
	tetris_map.cells[9] = 0;
	tetris_map.cells[10] = 1;
	tetris_map.cells[11] = 1;
	ASSERT_THAT(is_line_filled(&tetris_map, 1) == TETRIS_MAP_LINE_NOT_FILLED);
	ASSERT_THAT(is_line_filled(&tetris_map, 2) == TETRIS_MAP_LINE_NOT_FILLED);
	ASSERT_THAT(is_line_filled(&tetris_map, 3) == TETRIS_MAP_LINE_FILLED);
	ASSERT_THAT(is_line_filled(&tetris_map, 4) == TETRIS_MAP_LINE_NOT_FILLED);
	free_line(&tetris_map, 3);
	ASSERT_THAT(is_line_filled(&tetris_map, 1) == TETRIS_MAP_LINE_NOT_FILLED);
	ASSERT_THAT(is_line_filled(&tetris_map, 2) == TETRIS_MAP_LINE_NOT_FILLED);
	ASSERT_THAT(is_line_filled(&tetris_map, 3) == TETRIS_MAP_LINE_NOT_FILLED);
	ASSERT_THAT(is_line_filled(&tetris_map, 4) == TETRIS_MAP_LINE_NOT_FILLED);
	ASSERT_THAT(tetris_map.cells[0] == 0);
	ASSERT_THAT(tetris_map.cells[1] == 0);
	ASSERT_THAT(tetris_map.cells[2] == 0);
	ASSERT_THAT(tetris_map.cells[3] == 1);
	ASSERT_THAT(tetris_map.cells[4] == 0);
	ASSERT_THAT(tetris_map.cells[5] == 1);
	ASSERT_THAT(tetris_map.cells[6] == 1);
	ASSERT_THAT(tetris_map.cells[7] == 0);
	ASSERT_THAT(tetris_map.cells[8] == 0);
	ASSERT_THAT(tetris_map.cells[9] == 0);
	ASSERT_THAT(tetris_map.cells[10] == 1);
	ASSERT_THAT(tetris_map.cells[11] == 1);
	TETRIS_DESTROY(tetramino, tetris_map);
}
TEST(tetris_map_init)
{
	TETRIS_MAP_SETUP(tetris_map, 2, 3);
	ASSERT_THAT(tetris_map.cells != NULL);
	ASSERT_THAT(tetris_map.height == 2);
	ASSERT_THAT(tetris_map.width == 3);
	int i;
	for (i = 0; i < 2 * 3; i++)
	{
		ASSERT_THAT(tetris_map.cells[i] == 0);
	}
	TETRIS_MAP_DESTROY(tetris_map);
}
TEST(tetris_map_destroy)
{
	TETRIS_MAP_SETUP(tetris_map, 2, 3);
	TETRIS_MAP_DESTROY(tetris_map)
	ASSERT_THAT(tetris_map.cells == NULL);
	ASSERT_THAT(tetris_map.height == 0);
	ASSERT_THAT(tetris_map.width == 0);
}
TEST(tetramino_destroy)
{
	TETRIS_SETUP(tetramino, tetris_map, 2, 3);
	TETRIS_DESTROY(tetramino, tetris_map);
	ASSERT_THAT(tetris_map.cells == NULL);
	ASSERT_THAT(tetris_map.height == 0);
	ASSERT_THAT(tetris_map.width == 0);
	ASSERT_THAT(tetramino.x == 0);
	ASSERT_THAT(tetramino.y == 0);
}
TEST(tetramino_move_down)
{
	TETRIS_SETUP(tetramino, tetris_map, 1, 1);
	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_STOPPED);
	ASSERT_THAT(tetramino.y == 0);
	ASSERT_THAT(tetris_map.cells[0] != 0);
	TETRIS_DESTROY(tetramino, tetris_map);
}
TEST(tetramino_move_down_two_times)
{
	TETRIS_SETUP(tetramino, tetris_map, 2, 1);
	ASSERT_THAT(tetramino.x == 0);
	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_STOPPED);
	ASSERT_THAT(tetramino.y == 1);
	ASSERT_THAT(tetris_map.cells[0] == 0);
	ASSERT_THAT(tetris_map.cells[1] != 0);
	TETRIS_DESTROY(tetramino, tetris_map);
}
TEST(tetramino_can_move_down)
{
	TETRIS_SETUP(tetramino, tetris_map, 1, 1);
	ASSERT_THAT(can_tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_OK);
	tetramino.y++;
	ASSERT_THAT(can_tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_STOPPED);
	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_STOPPED);
	ASSERT_THAT(tetramino.y == 0);
	ASSERT_THAT(tetris_map.cells[0] != 0);
	TETRIS_DESTROY(tetramino, tetris_map);
}
TEST(tetramino_can_move_down_two_times)
{
	TETRIS_SETUP(tetramino, tetris_map, 2, 1);
	ASSERT_THAT(tetramino.x == 0);
	ASSERT_THAT(can_tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_OK);
	tetramino.y++;
	ASSERT_THAT(can_tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_OK);
	tetramino.y++;
	ASSERT_THAT(can_tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_STOPPED);
	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_STOPPED);
	ASSERT_THAT(tetramino.y == 1);
	ASSERT_THAT(tetris_map.cells[0] == 0);
	ASSERT_THAT(tetris_map.cells[1] != 0);
	TETRIS_DESTROY(tetramino, tetris_map);
}

TEST(tetramino_move_left)
{
	TETRIS_SETUP(tetramino, tetris_map, 1, 3);
	tetramino.x = 1;

	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_OK);

	ASSERT_THAT(tetramino_move_left(&tetramino, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramino_move_left(&tetramino, &tetris_map) == TETRAMINO_BLOCKED);
	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_STOPPED);
	ASSERT_THAT(tetramino.y == 0);
	ASSERT_THAT(tetramino.x == 0);
	ASSERT_THAT(tetris_map.cells[0] != 0);
	ASSERT_THAT(tetris_map.cells[1] == 0);
	TETRIS_DESTROY(tetramino, tetris_map);
}
TEST(tetramino_move_left_two_times)
{
	TETRIS_SETUP(tetramino, tetris_map, 1, 4);
	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_OK);
	tetris_map.cells[0] = 1;
	tetramino.x = 3;
	ASSERT_THAT(tetramino_move_left(&tetramino, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramino_move_left(&tetramino, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramino_move_left(&tetramino, &tetris_map) == TETRAMINO_BLOCKED);
	ASSERT_THAT(tetramino.y == 0);
	ASSERT_THAT(tetramino.x == 1);
	ASSERT_THAT(tetris_map.cells[0] != 0);
	ASSERT_THAT(tetris_map.cells[1] == 0);
	TETRIS_DESTROY(tetramino, tetris_map);
}
TEST(tetramino_can_move_left)
{
	TETRIS_SETUP(tetramino, tetris_map, 1, 2);
	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_OK);
	tetramino.x = 1;
	ASSERT_THAT(can_tetramino_move_left(&tetramino, &tetris_map) == TETRAMINO_OK);
	tetramino.x--;
	ASSERT_THAT(can_tetramino_move_left(&tetramino, &tetris_map) == TETRAMINO_BLOCKED);
	ASSERT_THAT(tetramino_move_left(&tetramino, &tetris_map) == TETRAMINO_BLOCKED);

	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_STOPPED);

	ASSERT_THAT(tetramino.y == 0);
	ASSERT_THAT(tetramino.x == 0);
	ASSERT_THAT(tetris_map.cells[0] != 0);
	TETRIS_DESTROY(tetramino, tetris_map);
}
TEST(tetramino_can_move_left_two_times)
{
	TETRIS_SETUP(tetramino, tetris_map, 1, 3);
	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_OK);
	tetramino.x = 2;
	ASSERT_THAT(can_tetramino_move_left(&tetramino, &tetris_map) == TETRAMINO_OK);
	tetramino.x--;
	ASSERT_THAT(can_tetramino_move_left(&tetramino, &tetris_map) == TETRAMINO_OK);
	tetramino.x--;
	ASSERT_THAT(can_tetramino_move_left(&tetramino, &tetris_map) == TETRAMINO_BLOCKED);
	ASSERT_THAT(tetramino_move_left(&tetramino, &tetris_map) == TETRAMINO_BLOCKED);

	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_STOPPED);

	ASSERT_THAT(tetramino.y == 0);
	ASSERT_THAT(tetramino.x == 0);
	ASSERT_THAT(tetris_map.cells[0] != 0);
	ASSERT_THAT(tetris_map.cells[1] == 0);
	TETRIS_DESTROY(tetramino, tetris_map);
}

TEST(tetramino_move_right)
{
	TETRIS_SETUP(tetramino, tetris_map, 1, 3);
	tetramino.x = 1;

	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_OK);

	ASSERT_THAT(tetramino_move_right(&tetramino, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramino_move_right(&tetramino, &tetris_map) == TETRAMINO_BLOCKED);
	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_STOPPED);
	ASSERT_THAT(tetramino.y == 0);
	ASSERT_THAT(tetramino.x == 2);
	ASSERT_THAT(tetris_map.cells[0] == 0);
	ASSERT_THAT(tetris_map.cells[1] == 0);
	ASSERT_THAT(tetris_map.cells[2] != 0);
	TETRIS_DESTROY(tetramino, tetris_map);
}
TEST(tetramino_move_right_two_times)
{
	TETRIS_SETUP(tetramino, tetris_map, 1, 4);
	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_OK);
	tetris_map.cells[3] = 1;
	tetramino.x = 0;
	ASSERT_THAT(tetramino_move_right(&tetramino, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramino_move_right(&tetramino, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramino_move_right(&tetramino, &tetris_map) == TETRAMINO_BLOCKED);
	ASSERT_THAT(tetramino.y == 0);
	ASSERT_THAT(tetramino.x == 2);
	ASSERT_THAT(tetris_map.cells[3] != 0);
	ASSERT_THAT(tetris_map.cells[2] == 0);
	ASSERT_THAT(tetris_map.cells[1] == 0);
	TETRIS_DESTROY(tetramino, tetris_map);
}
TEST(tetramino_can_move_right)
{
	TETRIS_SETUP(tetramino, tetris_map, 1, 2);
	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_OK);
	tetramino.x = 0;
	ASSERT_THAT(can_tetramino_move_right(&tetramino, &tetris_map) == TETRAMINO_OK);
	tetramino.x++;
	ASSERT_THAT(can_tetramino_move_right(&tetramino, &tetris_map) == TETRAMINO_BLOCKED);
	ASSERT_THAT(tetramino_move_right(&tetramino, &tetris_map) == TETRAMINO_BLOCKED);

	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_STOPPED);

	ASSERT_THAT(tetramino.y == 0);
	ASSERT_THAT(tetramino.x == 1);
	ASSERT_THAT(tetris_map.cells[1] != 0);
	ASSERT_THAT(tetris_map.cells[0] == 0);
	TETRIS_DESTROY(tetramino, tetris_map);
}
TEST(tetramino_can_move_right_two_times)
{
	TETRIS_SETUP(tetramino, tetris_map, 1, 3);
	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_OK);
	tetramino.x = 0;
	ASSERT_THAT(can_tetramino_move_right(&tetramino, &tetris_map) == TETRAMINO_OK);
	tetramino.x++;
	ASSERT_THAT(can_tetramino_move_right(&tetramino, &tetris_map) == TETRAMINO_OK);
	tetramino.x++;
	ASSERT_THAT(can_tetramino_move_right(&tetramino, &tetris_map) == TETRAMINO_BLOCKED);
	ASSERT_THAT(tetramino_move_right(&tetramino, &tetris_map) == TETRAMINO_BLOCKED);

	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_STOPPED);

	ASSERT_THAT(tetramino.y == 0);
	ASSERT_THAT(tetramino.x == 2);
	ASSERT_THAT(tetris_map.cells[2] != 0);
	ASSERT_THAT(tetris_map.cells[1] == 0);
	ASSERT_THAT(tetris_map.cells[0] == 0);
	TETRIS_DESTROY(tetramino, tetris_map);
}

TEST(tetramino_move_down_reached_bottom)
{
	TETRIS_SETUP(tetramino, tetris_map, 3, 2);
	tetramino.x = 1;
	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_STOPPED);
	ASSERT_THAT(tetramino.y == 2);
	ASSERT_THAT(tetramino.x == 1);
	ASSERT_THAT(tetris_map.cells[1] == 0);
	ASSERT_THAT(tetris_map.cells[3] == 0);
	ASSERT_THAT(tetris_map.cells[5] != 0);
	TETRIS_DESTROY(tetramino, tetris_map);
}
TEST(tetramino_move_down_map_full)
{
	TETRIS_SETUP(tetramino_first, tetris_map, 3, 3);
	tetramino_first.x = 0;
	TETRAMINO_SETUP(tetramino_other, tetris_map);
	tetramino_other.x = 0;
	ASSERT_THAT(tetramino_move_down(&tetramino_first, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramino_move_down(&tetramino_first, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramino_move_down(&tetramino_first, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramino_move_down(&tetramino_first, &tetris_map) == TETRAMINO_STOPPED);
	ASSERT_THAT(tetramino_move_down(&tetramino_other, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramino_move_down(&tetramino_other, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramino_move_down(&tetramino_other, &tetris_map) == TETRAMINO_STOPPED);
	ASSERT_THAT(tetramino_first.y == 2);
	ASSERT_THAT(tetramino_first.x == 0);
	ASSERT_THAT(tetramino_other.y == 1);
	ASSERT_THAT(tetramino_other.x == 0);
	ASSERT_THAT(tetris_map.cells[0] == 0);
	ASSERT_THAT(tetris_map.cells[3] != 0);
	ASSERT_THAT(tetris_map.cells[6] != 0);
	TETRIS_DESTROY(tetramino_first, tetris_map);
	TETRAMINO_DESTROY(tetramino_other);
}
TEST(tetramino_stopped)
{
	TETRIS_SETUP(tetramino, tetris_map, 3, 1);
	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_OK);
	ASSERT_THAT(tetramino_move_down(&tetramino, &tetris_map) == TETRAMINO_STOPPED);
	TETRIS_DESTROY(tetramino, tetris_map);
}

int main(int argc, char **argv)
{
	RUN_TEST(is_line_filled_custom_line);
	RUN_TEST(tetramino_init);
	RUN_TEST(tetramino_stopped);
	RUN_TEST(tetramino_move_down);
	RUN_TEST(tetramino_move_down_two_times);
	RUN_TEST(tetramino_can_move_down);
	RUN_TEST(tetramino_can_move_down_two_times);
	RUN_TEST(tetramino_move_left);
	RUN_TEST(tetramino_move_left_two_times);
	RUN_TEST(tetramino_can_move_left);
	RUN_TEST(tetramino_can_move_left_two_times);
	RUN_TEST(tetramino_move_right);
	RUN_TEST(tetramino_move_right_two_times);
	RUN_TEST(tetramino_can_move_right);
	RUN_TEST(tetramino_can_move_right_two_times);
	RUN_TEST(tetris_map_init);
	RUN_TEST(tetramino_move_down_map_full);
	RUN_TEST(tetramino_move_down_reached_bottom);
	RUN_TEST(tetris_map_destroy);
	RUN_TEST(tetramino_destroy);
	RUN_TEST(precise_rand);
	RUN_TEST(quick_rand_range);
	RUN_TEST(is_line_filled);
	RUN_TEST(free_filled_lines);
	RUN_TEST(free_line);
	RUN_TEST(free_line_custom_value);
	RUN_TEST(copy_tetramino);
	RUN_TEST(color_rgba_8_copy);
	RUN_TEST(set_color_rgba_8);
	RUN_TEST(set_random_color_rgba_8);
	RUN_TEST(tetramino_draw);
	RUN_TEST(tetramini_draw);
	RUN_TEST(tetramini_draw_offset);
	RUN_TEST(tetris_map_draw);
	RUN_TEST(_tetris_draw_internal);
	RUN_TEST(tetramini_init_0);
	RUN_TEST(tetramini_init_2);
	RUN_TEST(tetramini_init_6);
	RUN_TEST(tetramini_init_random);
	RUN_TEST(tetramini_destroy);
	RUN_TEST(tetramini_copy);
	RUN_TEST(tetramini_rotate);
	RUN_TEST(tetramini_rotate_to_end);
	RUN_TEST(tetramini_rotate_to_border);
	RUN_TEST(can_tetramini_move_down);
	RUN_TEST(can_tetramini_move_left);
	RUN_TEST(can_tetramini_move_right);
	RUN_TEST(can_tetramini_move_down_till_end);
	RUN_TEST(can_tetramini_move_left_till_end);
	RUN_TEST(can_tetramini_move_right_till_end);
	RUN_TEST(can_tetramini_move_down_till_border);
	RUN_TEST(can_tetramini_move_left_till_border);
	RUN_TEST(can_tetramini_move_right_till_border);
	RUN_TEST(tetramini_move_down);
	RUN_TEST(tetramini_move_left);
	RUN_TEST(tetramini_move_right);
	RUN_TEST(tetramini_move_down_till_end);
	RUN_TEST(tetramini_move_left_till_end);
	RUN_TEST(tetramini_move_right_till_end);
	RUN_TEST(tetramini_move_down_till_border);
	RUN_TEST(tetramini_move_left_till_border);
	RUN_TEST(tetramini_move_right_till_border);
	RUN_TEST(set_color_rgba_8_from_tetramini_type);
	RUN_TEST(get_type_data);
	RUN_TEST(min_value);
	RUN_TEST(max_value);

	PRINT_TEST_RESULTS();
	return 0;
}
