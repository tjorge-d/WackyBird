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

static void game_restart(t_game *game)
{
	for (int i = 0; i < MAX_OBS; i++)
		game->obstacles[i].active = 0;
	game->score = -1;
	game->bird.y = Y_RES / 2;
	game->bird.x = X_RES * BIRD_X;
	game->bird.v = 0;
	game->phys.game_speed = Y_RES * OBS_SPEED;
	game->bird.alive = 1;
	game->layout.menu_p = Y_RES;
	game->bird.img = &game->img[BIRDUP];
}

static void game_start(t_game *game)
{
	game->score = 0;
	game->bird.v = -game->phys.bird_jump;
	game->bird.x = X_RES * BIRD_X;
	game->running = 1;
}

static int	key_press(int keycode, t_game *game)
{
	if (!game->bird.alive && !game->running && !(game->layout.menu_p > game->layout.menu_y) && keycode == FORE)
		game_restart(game);
	else if (!game->running && !(game->layout.menu_p > game->layout.menu_y) && keycode == FORE)
		game_start(game);
	else if (game->running && keycode == FORE)
		game->bird.v = -game->phys.bird_jump;
	else if (keycode == ESC)
		game_close(game, 2);
	return (0);
}

int	main()
{
	static t_game	game;

	load_game(&game);
	mlx_key_hook(game.window, &key_press, &game);
	mlx_hook(game.window, 17, StructureNotifyMask, &game_close, &game);
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_loop(game.mlx);
	game_close(&game, 2);
}