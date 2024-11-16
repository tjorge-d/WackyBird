#include "WackyBird.h"

static void multiply_background_pixel(t_game *game, int stretched_b, int x, int y)
{
	for (int i = 1; i < BACKGROUND_LENGHT; i++)
	{
		my_mlx_pixel_force(&game->img[stretched_b], x + (i * Y_RES), y, \
		get_color(&game->img[stretched_b], x, y));
	}
}

static void create_background(t_game *game, int b_num)
{
	float x = 0;
	float y = 0;
	float ratio = (Y_RES / (float)game->img[b_num].w);


	while  (y < Y_RES)
	{
		x = 0;
		while (x < Y_RES)
		{
			my_mlx_pixel_force(&game->img[b_num + ASSET_NUMBER], x, y, \
			get_color(&game->img[b_num], (x / ratio), (y / ratio)));
			multiply_background_pixel(game, b_num + ASSET_NUMBER, x, y);
			x++;
		}
		y++;
	}
}

static void	set_vars(t_game *game)
{
	static t_image			img[ASSET_NUMBER + IMG_NUMBER];
	static t_bird			bird;
	static t_obstacles		obstacles[MAX_OBS];
	static t_settings		settings;
	static t_layout			layout;

	game->img = img;
	game->bird = bird;
	game->obstacles = obstacles;
	game->settings = settings;
	game->layout = layout;
	game->bird.y = Y_RES / 2;
	game->bird.v = 0;
	game->layout.obs_s = OBS_START_SPEED;
	game->layout.next_obs_inc = game->layout.obs_s + OBS_INC_RATE;
	game->layout.obs_n = 1;
	game->img[FRAME].h = Y_RES;
	game->img[FRAME].w = X_RES;
	game->img[B1].h = Y_RES;
	game->img[B1].w = Y_RES * BACKGROUND_LENGHT;
	game->img[B2].h = Y_RES;
	game->img[B2].w = Y_RES * BACKGROUND_LENGHT;
	game->img[B3].h = Y_RES;
	game->img[B3].w = Y_RES * BACKGROUND_LENGHT;
}

static void	set_asset_paths(t_game *game)
{
	game->img[BIRD].path = "./Assets/mini_mario.xpm";
	game->img[BACKGROUND1].path = "./Assets/bale_wall_1.xpm";
	game->img[BACKGROUND2].path = "./Assets/mini_mario.xpm";
	game->img[BACKGROUND3].path = "./Assets/mini_open_door.xpm";
	game->img[OBSTACLE].path = "./Assets/castle_wall.xpm";
}

void load_game(t_game *game)
{
	set_vars(game);
	set_asset_paths(game);
	game->mlx = mlx_init();
	gettimeofday(&game->time, NULL);
	game->last_frame = game->time.tv_sec * 1000000 + game->time.tv_usec;
	game->window = mlx_new_window(game->mlx, X_RES, Y_RES, "Wacky Bird");
	for (int i = 0 ; i < ASSET_NUMBER + IMG_NUMBER; i++)
	{
		if (i >= ASSET_NUMBER)
			game->img[i].img = mlx_new_image(game->mlx, game->img[i].w, \
				game->img[i].h);
		else
		{
			game->img[i].img = mlx_xpm_file_to_image(game->mlx, \
				game->img[i].path, &game->img[i].w, &game->img[i].h);
		}
		if (!game->img[i].img)
			game_close(game, 2);
		game->img[i].addr = mlx_get_data_addr(game->img[i].img, \
			&game->img[i].bpp, &game->img[i].line_length, &game->img[i].endian);
		game->img[i].created = 1;
	}
	create_background(game, 1);
	create_background(game, 2);
	create_background(game, 3);
	game->bird.img = game->img[OBSTACLE];
}