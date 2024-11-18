#include "WackyBird.h"

static void set_image_values(t_game *game)
{
	game->img[FRAME].h = Y_RES;
	game->img[FRAME].w = X_RES;
	game->img[B1].h = Y_RES;
	game->img[B2].h = Y_RES;
	game->img[B3].h = Y_RES;
	game->img[B1].w = Y_RES * BACKGROUND_LENGHT;
	game->img[B2].w = Y_RES * BACKGROUND_LENGHT;
	game->img[B3].w = Y_RES * BACKGROUND_LENGHT;
	game->img[BOTTOM_OBS].w = Y_RES * OBS_WIDTH;
	game->img[TOP_OBS].w = Y_RES * OBS_WIDTH;
	game->img[BOTTOM_OBS].h = game->img[BOTTOM_OBS].w * OBS_HEIGHT;
	game->img[TOP_OBS].h = game->img[TOP_OBS].w * OBS_HEIGHT;
}

static void set_layout(t_game *game)
{
	game->layout.menu_speed = Y_RES * MENU_SPEED;
	game->layout.menu_p = Y_RES;
	game->layout.menu_x = 0;
	game->layout.menu_y = (Y_RES - X_RES) / 2;
	game->img[GAMEOVER].scale = X_RES / (float)game->img[GAMEOVER].w;
	game->img[GAMESTART].scale = X_RES / (float)game->img[GAMESTART].w;
	if (X_RES > Y_RES)
	{
		game->layout.menu_x =(X_RES - Y_RES) / 2;
		game->layout.menu_y = 0;
		game->img[GAMEOVER].scale = Y_RES / (float)game->img[GAMEOVER].w;
		game->img[GAMESTART].scale = Y_RES / (float)game->img[GAMESTART].w;
	}
	game->img[BIRD].scale = (float)Y_RES * BIRD_SIZE;
	game->bird.h = game->img[BIRD].h * game->img[BIRD].scale;
	game->bird.w = game->img[BIRD].w * game->img[BIRD].scale;
	game->bird.img = game->img[BIRD];
}

static void	set_vars(t_game *game)
{
	static t_image			img[ASSET_NUMBER + IMG_NUMBER];
	static t_bird			bird;
	static t_obstacles		obstacles[MAX_OBS];
	static t_phys			phys;
	static t_layout			layout;

	game->img = img;
	game->bird = bird;
	game->obstacles = obstacles;
	game->phys = phys;
	game->layout = layout;
	game->running = 0;
	game->score = -1;
	game->bird.x = X_RES * BIRD_X;
	game->bird.y = Y_RES / 2;
	game->bird.v = 0;
	game->bird.alive = 1;
	game->bird.knockback = Y_RES * BIRD_KNOCKBACK;
	set_image_values(game);
	game->phys.bird_ac = Y_RES * ACCELERATION;
	game->phys.bird_jump = Y_RES * JUMP_STR;
	game->phys.game_speed = Y_RES * OBS_SPEED;
	game->phys.obs_ac = game->phys.game_speed / 60;
	game->phys.obs_distance = Y_RES * OBS_DISTANCE;
	game->phys.obs_gap = Y_RES * OBS_GAP;
}

static void	set_asset_paths(t_game *game)
{
	game->img[BIRD].path = "./Assets/mini_mario.xpm";
	game->img[BACKGROUND1].path = "./Assets/bale_wall_1.xpm";
	game->img[BACKGROUND2].path = "./Assets/mini_mario.xpm";
	game->img[BACKGROUND3].path = "./Assets/mini_open_door.xpm";
	game->img[OBSTACLE].path = "./Assets/castle_wall.xpm";
	game->img[OBSTACLE_END].path = "./Assets/snow_wall.xpm";
	game->img[GAMESTART].path = "./Assets/mini_open_door.xpm";
	game->img[GAMEOVER].path = "./Assets/milos_wall_1.xpm";
}

static void multiply_obstacles_pixel(t_game *game, int x, int y)
{
	for (int i = 1; i < OBS_HEIGHT - 1 ; i++)
	{
		my_mlx_pixel_force(&game->img[BOTTOM_OBS], x , y + (i * game->img[BOTTOM_OBS].w), \
		get_color(&game->img[BOTTOM_OBS], x, y));
		my_mlx_pixel_force(&game->img[TOP_OBS], game->img[TOP_OBS].w - x , \
		game->img[TOP_OBS].h - y - (i * game->img[TOP_OBS].w), \
		get_color(&game->img[BOTTOM_OBS], x, y));
	}
}

static void fill_obstacles(t_game *game)
{
	float x = 0;
	float y = 0;
	float ratio = ((float)game->img[BOTTOM_OBS].w / (float)game->img[OBSTACLE].w);
	float end_ratio = ((float)game->img[BOTTOM_OBS].w / (float)game->img[OBSTACLE_END].w);

	while  (y < game->img[BOTTOM_OBS].w)
	{
		x = 0;
		while (x < game->img[BOTTOM_OBS].w)
		{
			my_mlx_pixel_force(&game->img[BOTTOM_OBS], x, y + game->img[BOTTOM_OBS].w, \
			get_color(&game->img[OBSTACLE], (x / ratio), (y / ratio)));
			my_mlx_pixel_force(&game->img[BOTTOM_OBS], x, y, \
			get_color(&game->img[OBSTACLE_END], (x / end_ratio), (y / end_ratio)));
			my_mlx_pixel_force(&game->img[TOP_OBS], game->img[TOP_OBS].w - x, \
			game->img[TOP_OBS].h - y - game->img[BOTTOM_OBS].w, \
			get_color(&game->img[OBSTACLE], (x / ratio), (y / ratio)));
			my_mlx_pixel_force(&game->img[TOP_OBS], game->img[TOP_OBS].w - x, \
			game->img[TOP_OBS].h - y, \
			get_color(&game->img[OBSTACLE_END], (x / end_ratio), (y / end_ratio)));
			multiply_obstacles_pixel(game, x, y + game->img[BOTTOM_OBS].w);
			x++;
		}
		y++;
	}
}

static void multiply_background_pixel(t_game *game, int stretched_b, int x, int y)
{
	for (int i = 1; i < BACKGROUND_LENGHT; i++)
	{
		my_mlx_pixel_force(&game->img[stretched_b], x + (i * Y_RES), y, \
		get_color(&game->img[stretched_b], x, y));
	}
}

static void fill_background(t_game *game, int i)
{
	float x = 0;
	float y = 0;
	float ratio = (Y_RES / (float)game->img[i].h);

	if (i > 3)
		return ;
	while  (y < Y_RES)
	{
		x = 0;
		while (x < Y_RES)
		{
			my_mlx_pixel_force(&game->img[i + ASSET_NUMBER], x, y, \
			get_color(&game->img[i], (x / ratio), (y / ratio)));
			multiply_background_pixel(game, i + ASSET_NUMBER, x, y);
			x++;
		}
		y++;
	}
	fill_background(game, i + 1);
}

static void create_images(t_game *game)
{
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
		game->img[i].scale = 1;
	}
}

void load_game(t_game *game)
{
	srand(time(NULL));
	set_vars(game);
	set_asset_paths(game);
	game->mlx = mlx_init();
	gettimeofday(&game->time, NULL);
	game->last_frame = game->time.tv_sec * 1000000 + game->time.tv_usec;
	game->window = mlx_new_window(game->mlx, X_RES, Y_RES, "Wacky Bird");
	create_images(game);
	fill_background(game, 1);
	fill_obstacles(game);
	set_layout(game);
}