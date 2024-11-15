#include "WackyBird.h"

int	game_close(t_game *game, int exit_code)
{
	mlx_destroy_window(game->mlx, game->window);
	for (int i = 0 ; i < ASSET_NUMBER + IMG_NUMBER; i++)
	{
		if (game->img[i].created)
			mlx_destroy_image(game->mlx, game->img[i].img); 
	}
	mlx_destroy_display(game->mlx);
	free(game->mlx);
	exit(exit_code);
}

static void	set_vars(t_game *game)
{
	static t_image			img[ASSET_NUMBER + IMG_NUMBER];
	static t_bird			bird;
	static t_obstacles		obstacles[10];
	static t_settings		settings;
	static t_layout			layout;

	game->img = img;
	game->bird = bird;
	game->obstacles = obstacles;
	game->ob_s = 30;
	game->ob_a = 1;
	game->settings = settings;
	game->layout = layout;
	game->bird.y = Y_RES / 2;
	game->bird.v = 0;
	game->bird.a = 98;
	game->layout.b1s = 0.15;
	game->layout.b1p = 0;
	game->layout.b2s = 0.45;
	game->layout.b2p = 0;
	game->layout.b3s = 0.80;
	game->layout.b3p = 0;
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
	game->img[BACKGROUND1].path = "./Assets/snow_wall.xpm";
	game->img[BACKGROUND2].path = "./Assets/sigma_wall_1.xpm";
	game->img[BACKGROUND3].path = "./Assets/pineapple_wall.xpm";
	game->img[OBSTACLE].path = "./Assets/castle_wall.xpm";
}

unsigned int	get_color(t_image *img, int x, int y)
{
	char	*dst;

	dst = img->addr + (y * img->line_length + x * (img->bpp / 8));
	return (*(unsigned int *)dst);
}

void	my_mlx_pixel_put(t_image *img, int x, int y, int color)
{
	char	*dst;

	if (color == TRANSPARENT)
		return ;

	dst = img->addr + (y * img->line_length + x * (img->bpp / 8));
	*(unsigned int *)dst = color;
}

void multiply_background_pixel(t_game *game, int stretched_b, int x, int y)
{
	for (int i = 1; i < BACKGROUND_LENGHT; i++)
	{
		my_mlx_pixel_put(&game->img[stretched_b], x + (i * Y_RES), y, \
		get_color(&game->img[stretched_b], x, y));
	}
}

void create_background(t_game *game, int b_num)
{
	float x = 0;
	float y = 0;
	float ratio = (Y_RES / (float)game->img[b_num].w);


	while  (y < Y_RES)
	{
		x = 0;
		while (x < Y_RES)
		{
			my_mlx_pixel_put(&game->img[b_num + ASSET_NUMBER], x, y, \
			get_color(&game->img[b_num], (x / ratio), (y / ratio)));
			multiply_background_pixel(game, b_num + ASSET_NUMBER, x, y);
			x++;
		}
		y++;
	}
}

void	load_game(t_game *game)
{
	set_vars(game);
	set_asset_paths(game);
	game->mlx = mlx_init();
	gettimeofday(&game->time, NULL);
	game->last_frame = game->time.tv_sec * 1000000 + game->time.tv_usec;
	game->window = mlx_new_window(game->mlx, X_RES, Y_RES, "Window");
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
	game->bird.img = game->img[BIRD];
}

int	key_press(int keycode, t_game *game)
{
	if (keycode == FORE)
		game->bird.v = -JUMP_STR;
	else if (keycode == ESC)
		game_close(game, 2);
	return (0);
}

void	bird_movement(t_game *game)
{
	game->bird.y += game->bird.v / FRAME_RATE;
	game->bird.v += game->bird.a / FRAME_RATE;
	if (game->bird.y >= Y_RES)
	{
		game_close(game, 2);
		printf("GAY!\n");
	}
}

void	image_to_frame(t_game *game, t_image image, int x, int y)
{
	int		x_pos = x;
	int		y_pos = y;
	int		scale = image.scale;

	if (x_pos < 0)
		x_pos = 0;
	if (y_pos < 0)
		y_pos = 0;
	y = y_pos;
	if (!scale)
		scale = 1;
	while (y < y_pos + (image.h * scale) && y < Y_RES)
	{
		x = x_pos;
		while (x < x_pos + (image.w * scale) && x < X_RES)
		{
			my_mlx_pixel_put(&game->img[FRAME], x, y, \
			get_color(&image, (x - x_pos) / scale, \
			(y - y_pos) / scale));
			x++;
		}
		y++;
	}
}

void	build_frame(t_game *game)
{
	image_to_frame(game, game->img[B2], 0, 0);
	image_to_frame(game, game->bird.img, \
		X_RES / 2 - game->bird.img.w, game->bird.y - game->bird.img.h);
	mlx_put_image_to_window(game->mlx, game->window, game->img[FRAME].img, 0, 0);
}

/*obstacles(t_game *game)
{

}*/

int	game_loop(t_game *game)
{
	gettimeofday(&game->time, NULL);
	if ((game->time.tv_sec * 1000000 + \
	game->time.tv_usec) > game->last_frame)
	{
		bird_movement(game);
		//obstacles(game);
		build_frame(game);
		gettimeofday(&game->time, NULL);
		game->last_frame = game->time.tv_sec * 1000000 \
		+ game->time.tv_usec + (1000000 / FRAME_RATE);
	}
	return (1);
}



int	main()
{
	static t_game	game;

	load_game(&game);
	mlx_key_hook(game.window, &key_press, &game);
	mlx_hook(game.window, 17, StructureNotifyMask, &game_close, &game);
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_loop(game.mlx);
	return (0);
	//game_close(&game, 2);
}