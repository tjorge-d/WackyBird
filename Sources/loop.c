#include "WackyBird.h"

void bird_movement(t_game *game)
{
	game->bird.y += (game->bird.v / (float)FRAME_RATE);
	game->bird.v += ((float)ACCELERATION / (float)FRAME_RATE);
	image_to_frame(game, game->bird.img, X_RES / 2, game->bird.y);
	if (game->bird.y >= Y_RES)
	{
		printf("GAY!\n");
		game_close(game, 2);
	}
}

void generate_random_gap(t_game *game, int obs_n)
{
	game->obstacles[obs_n].gap_start = \
	((float)Y_RES / (float)16) + rand() % (int)(((float)Y_RES / (float)16) * 15 - ((float)Y_RES / (float)16) + 1);
}

void print_obstacles(t_game *game, int i)
{
	for (int y = game->obstacles[i].top_y; y > -game->img[OBSTACLE].h; y -= game->img[OBSTACLE].h)
		image_to_frame(game, game->img[OBSTACLE], game->obstacles[i].x, y);
	for (int y = game->obstacles[i].bottom_y; y < Y_RES; y += game->img[OBSTACLE].h)
		image_to_frame(game, game->img[OBSTACLE], game->obstacles[i].x, y);
}

void obstacles(t_game *game)
{
	for (int i = 0; i < MAX_OBS; i++)
	{
		if (game->obstacles[i].active)
		{
			if (!(game->obstacles[i].x + game->img[OBSTACLE].w <= X_RES / 2 || \
				game->obstacles[i].x >= X_RES / 2 + game->bird.img.w || \
				game->obstacles[i].bottom_y + game->obstacles[i].bottom_h <= game->bird.y || \
				game->obstacles[i].bottom_y >= game->bird.y + game->bird.img.h) || \
				!(game->obstacles[i].x + game->img[OBSTACLE].w <= X_RES / 2 || \
				game->obstacles[i].x >= X_RES / 2 + game->bird.img.w || \
				0 + game->obstacles[i].top_h <= game->bird.y || \
				0 >= game->bird.y + game->bird.img.h))
				printf("PANELEIROO!\n");
			game->obstacles[i].x -= (game->layout.obs_s / (float)FRAME_RATE);
			if (game->obstacles[i].x + game->img[OBSTACLE].w < 0)
				game->obstacles[i].active = 0;
			print_obstacles(game, i);
		}
	}
	game->layout.obs_s += ((float)OBS_AC / (float)FRAME_RATE);
	gettimeofday(&game->time, NULL);
	if ((game->time.tv_sec * 1000000 + game->time.tv_usec) > game->layout.next_obs_inc)
	{
		game->layout.next_obs_inc = game->time.tv_sec * 1000000 + game->time.tv_usec + 1000000 * OBS_FREQ;
		for (int i = 0; i < MAX_OBS; i++)
		{
			if (game->obstacles[i].active == 0)
			{
				game->obstacles[i].active = 1;
				game->obstacles[i].x = X_RES;
				generate_random_gap(game, i);
				game->obstacles[i].top_y = game->obstacles[i].gap_start - game->img[OBSTACLE].h;
				game->obstacles[i].bottom_y = game->obstacles[i].gap_start + (float)OBS_GAP;
				game->obstacles[i].top_h = game->obstacles[i].gap_start;
				game->obstacles[i].bottom_h = (float)Y_RES - game->obstacles[i].bottom_y; 
				break;
			}
		}
	}
}

void background(t_game *game)
{
	image_to_frame(game, game->img[B1], game->layout.b1p, 0);
	game->layout.b1p -= (game->layout.obs_s / (float)FRAME_RATE) * (float)B1_SPEED_RATIO;
	if (game->layout.b1p <= -Y_RES)
		game->layout.b1p = 0;
	image_to_frame(game, game->img[B2], game->layout.b2p, 0);
	game->layout.b2p -= (game->layout.obs_s / (float)FRAME_RATE) * (float)B2_SPEED_RATIO;
	if (game->layout.b2p <= -Y_RES)
		game->layout.b2p = 0;
	image_to_frame(game, game->img[B3], game->layout.b3p, 0);
	game->layout.b3p -= (game->layout.obs_s / (float)FRAME_RATE) * (float)B3_SPEED_RATIO;
	if (game->layout.b3p <= -Y_RES)
		game->layout.b3p = 0;
}