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

static int	key_press(int keycode, t_game *game)
{
	if (keycode == FORE)
		game->bird.v = -JUMP_STR;
	else if (keycode == ESC)
		game_close(game, 2);
	return (0);
}

int	game_loop(t_game *game)
{
	gettimeofday(&game->time, NULL);
	if ((game->time.tv_sec * 1000000 + \
	game->time.tv_usec) > game->last_frame)
	{
		background(game);
		bird_movement(game);
		obstacles(game);
		mlx_put_image_to_window(game->mlx, game->window, game->img[FRAME].img, 0, 0);
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