#include "tetris.h"
#include "gfx.h"
#define SCREEN_W 960
#define SCREEN_H 540
#define TETRIS_MAP_W 10
#define TETRIS_MAP_H 20
#define TETRIS_TIMER_MS 1000
#define TEXT_FOR_SIMPLE_TETRIS "single"

//#define USE_SINGLE_TETRAMINO
//#undef USE_SINGLE_TETRAMINO

//#ifdef USE_SINGLE_TETRAMINO
int move_down_tetramino(tetramino *const tetramino_scene, tetramino *const tetramino_preview, tetris_map *const tetris_map, color_rgba_8 *const tetramino_color, color_rgba_8 *const tetramino_preview_color, Uint32 *const last_ticks, const Uint32 *const current_ticks)
{
	*last_ticks = *current_ticks;
	if (tetramino_move_down(tetramino_scene, tetris_map) == TETRAMINO_STOPPED)
	{
		if (tetramino_scene->y == -1)
		{
			return 1;
		}
		free_filled_lines(tetris_map, tetramino_scene->y + 1, tetramino_scene->y + 1);

		color_rgba_8_copy(tetramino_color, tetramino_preview_color);
		set_random_color_rgba_8(tetramino_preview_color, 50, 255, 255);
		tetramino_copy(tetramino_scene, tetramino_preview);

		tetramino_destroy(tetramino_preview);
		tetramino_init(tetramino_preview, tetris_map);

		//TODO: sound effect
	}
	return 0;
}
//#else
int move_down_tetramini(tetramini *const tetramini_scene, tetramini *const tetramini_preview, tetris_map *const tetris_map, Uint32 *const last_ticks, const Uint32 *const current_ticks)
{
	*last_ticks = *current_ticks;
	if (tetramini_move_down(tetramini_scene, tetris_map) == TETRAMINO_STOPPED)
	{
		register int j;
		int min_line = INT_MAX;
		int max_line = INT_MIN;
		for (j = 0; j < TETRAMINI_SIZE; j++)
		{
			const int line = tetramini_scene->arr_tetramini[j].y + 1;
			if (line <= 0)
			{
				return 1;
			}
			if (line < min_line)
			{
				min_line = line;
			}
			if (line > max_line)
			{
				max_line = line;
			}
		}
		free_filled_lines(tetris_map, min_line, max_line);

		tetramini_copy(tetramini_scene, tetramini_preview);

		tetramini_init(tetramini_preview, tetris_map, -1);

		//TODO: sound effect
	}
	return 0;
}
//#endif
int main(int argc, char **argv)
{
	int use_single_tetramino = 0;
	while (*argv)
	{
		if (strcmp(TEXT_FOR_SIMPLE_TETRIS, *argv++) == 0)
		{
			use_single_tetramino = 1;
			break;
		}
	}
	int ret_value = 0;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
	{
		SDL_Log("Unable to initialize sdl2: %s\n", SDL_GetError());
		ret_value = -1;
		goto cleanup;
	}

	SDL_Window *window = SDL_CreateWindow("Tetris_Cazzuto", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 960, 540, 0);
	if (!window)
	{
		SDL_Log("Unable to initialize window: %s\n", SDL_GetError());
		ret_value = -1;
		goto cleanup_sdl;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer)
	{
		SDL_Log("Unable to initialize renderer: %s\n", SDL_GetError());
		ret_value = -1;
		goto cleanup_window;
	}

	const int tetramino_square_size = MIN((SCREEN_W / TETRIS_MAP_W), (SCREEN_H / TETRIS_MAP_H));

	tetris_map tetris_map;
	tetris_map_init(&tetris_map, TETRIS_MAP_H, TETRIS_MAP_W);

	//#ifdef USE_SINGLE_TETRAMINO
	tetramino tetramino_scene;
	tetramino tetramino_preview;
	color_rgba_8 color;
	color_rgba_8 preview_color;

	tetramini tetramini_scene;
	tetramini tetramini_preview;
	if (use_single_tetramino)
	{
		tetramino_init(&tetramino_scene, &tetris_map);
		tetramino_init(&tetramino_preview, &tetris_map);
		set_random_color_rgba_8(&color, 50, 255, 255);
		set_random_color_rgba_8(&preview_color, 50, 255, 255);
	}
	else
	{
		//#else
		tetramini_init(&tetramini_scene, &tetris_map, -1);
		tetramini_init(&tetramini_preview, &tetris_map, -1);
	}
	//#endif

	color_rgba_8 tetris_map_color;
	set_color_rgba_8(&tetris_map_color, 0, 0, 255, 255);

	SDL_Rect preview_rect;
	preview_rect.x = SCREEN_W - tetramino_square_size * 3;
	preview_rect.y = tetramino_square_size * 4;
	preview_rect.h = tetramino_square_size;
	preview_rect.w = tetramino_square_size;

	Uint32 last_ticks = SDL_GetTicks();

	for (;;)
	{
		Uint32 current_ticks = SDL_GetTicks();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				goto cleanup_renderer;
			}
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_RIGHT)
				{
					//#ifdef USE_SINGLE_TETRAMINO
					if (use_single_tetramino)
					{

						if (tetramino_move_right(&tetramino_scene, &tetris_map) == TETRAMINO_BLOCKED)
						{
							//TODO: sound effect
						}
					}
					else
					{

						//#else
						if (tetramini_move_right(&tetramini_scene, &tetris_map) == TETRAMINO_BLOCKED)
						{
							//TODO: sound effect
						}
					}
					//#endif
				}
				if (event.key.keysym.sym == SDLK_LEFT)
				{
					//#ifdef USE_SINGLE_TETRAMINO
					if (use_single_tetramino)
					{

						if (tetramino_move_left(&tetramino_scene, &tetris_map) == TETRAMINO_BLOCKED)
						{
							//TODO: sound effect
						}
					}
					else
					{

						//#else
						if (tetramini_move_left(&tetramini_scene, &tetris_map) == TETRAMINO_BLOCKED)
						{
							//TODO: sound effect
						}
					}
					//#endif
				}
				if (event.key.keysym.sym == SDLK_DOWN)
				{
					//#ifdef USE_SINGLE_TETRAMINO
					if (use_single_tetramino)
					{

						if (move_down_tetramino(&tetramino_scene, &tetramino_preview, &tetris_map, &color, &preview_color, &last_ticks, &current_ticks))
						{
							goto cleanup_renderer;
						}
					}
					else
					{

						//#else
						if (move_down_tetramini(&tetramini_scene, &tetramini_preview, &tetris_map, &last_ticks, &current_ticks))
						{
							goto cleanup_renderer;
						}
					}
					//#endif
				}
				//#ifndef USE_SINGLE_TETRAMINO
				if (!use_single_tetramino)
				{

					if (event.key.keysym.sym == SDLK_UP)
					{
						if (tetramini_rotate(&tetramini_scene, &tetris_map) != TETRAMINO_OK)
						{
							//TODO: BOH,
						}
						else
						{
							//TODO: maybe sound?
						}
					}
				}
				//#endif
			}
		}

		if (current_ticks >= last_ticks + TETRIS_TIMER_MS)
		{
			//#ifdef USE_SINGLE_TETRAMINO
			if (use_single_tetramino)
			{

				if (move_down_tetramino(&tetramino_scene, &tetramino_preview, &tetris_map, &color, &preview_color, &last_ticks, &current_ticks))
				{
					goto cleanup_renderer;
				}
			}
			else
			{

				//#else
				if (move_down_tetramini(&tetramini_scene, &tetramini_preview, &tetris_map, &last_ticks, &current_ticks))
				{
					goto cleanup_renderer;
				}
			}
			//#endif
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		//#ifdef USE_SINGLE_TETRAMINO
		if (use_single_tetramino)
		{

			tetramino_draw(&tetramino_scene, renderer, tetramino_square_size, &color);
			_tetris_draw_internal(renderer, &preview_rect, &preview_color);
		}
		else
		{

			//#else
			tetramini_draw(&tetramini_scene, renderer, tetramino_square_size);
			tetramini_draw_offset(&tetramini_preview, renderer, tetramino_square_size, &preview_rect);
		}
		//#endif
		tetris_map_draw(&tetris_map, renderer, tetramino_square_size, &tetris_map_color);

		SDL_RenderPresent(renderer);
	}

cleanup_renderer:
	SDL_DestroyRenderer(renderer);
cleanup_window:
	SDL_DestroyWindow(window);
cleanup_sdl:
	SDL_Quit();
cleanup:
	return ret_value;
}
