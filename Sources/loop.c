#include "WackyBird.h"

static void generate_random_gap(t_game *game, int obs_n)
{
	game->obstacles[obs_n].gap_start = \
	rand() % (int)((Y_RES / 16) * 15 - game->phys.obs_gap - (Y_RES / 16) + 1) + (Y_RES / 16);
}

static void print_obstacles(t_game *game, int i)
{
	image_to_frame(game, game->img[TOP_OBS], game->obstacles[i].x, game->obstacles[i].top_y);
	image_to_frame(game, game->img[BOTTOM_OBS], game->obstacles[i].x, game->obstacles[i].bottom_y);
}

static void collision_check(t_game *game, int i)
{
	if (!(game->obstacles[i].x + game->img[BOTTOM_OBS].w <= game->bird.x || \
	game->obstacles[i].x >= game->bird.x + game->bird.w || \
	game->obstacles[i].bottom_y + game->img[BOTTOM_OBS].h <= game->bird.y || \
	game->obstacles[i].bottom_y >= game->bird.y + game->bird.h) || \
	!(game->obstacles[i].x + game->img[TOP_OBS].w <= game->bird.x || \
	game->obstacles[i].x >= game->bird.x + game->bird.w || \
	game->obstacles[i].top_y + game->img[TOP_OBS].h <= game->bird.y || \
	game->obstacles[i].top_y >= game->bird.y + game->bird.h))
	{
		game->running = 0 ;
		game->phys.obs_speed = 0;
		game->bird.alive = 0;
	}
}

static void score_check(t_game *game, int i)
{
	if (!game->obstacles[i].collected && game->obstacles[i].x + game->img[TOP_OBS].w / 2 < game->bird.x)
	{
		printf("GOOOOOL! %i\n", game->score);
		game->score++;
		game->obstacles[i].collected = 1;
	}
}

static void initialize_obstacle(t_game *game)
{
	for (int i = 0; i < MAX_OBS; i++)
	{
		if (game->obstacles[i].active == 0)
		{
			game->obstacles[i].active = 1;
			game->obstacles[i].collected = 0;
			game->obstacles[i].x = X_RES + game->img[BOTTOM_OBS].w;
			generate_random_gap(game, i);
			game->obstacles[i].top_y = game->obstacles[i].gap_start - game->img[TOP_OBS].h;
			game->obstacles[i].bottom_y = game->obstacles[i].gap_start + game->phys.obs_gap; 
			return;
		}
	}
}

void background(t_game *game)
{
	image_to_frame(game, game->img[B1], game->layout.b1p, 0);
	game->layout.b1p -= (game->phys.obs_speed / (float)FRAME_RATE) * (float)B1_SPEED_RATIO;
	if (game->layout.b1p <= -Y_RES)
		game->layout.b1p = 0;
	image_to_frame(game, game->img[B2], game->layout.b2p, 0);
	game->layout.b2p -= (game->phys.obs_speed / (float)FRAME_RATE) * (float)B2_SPEED_RATIO;
	if (game->layout.b2p <= -Y_RES)
		game->layout.b2p = 0;
	image_to_frame(game, game->img[B3], game->layout.b3p, 0);
	game->layout.b3p -= (game->phys.obs_speed / (float)FRAME_RATE) * (float)B3_SPEED_RATIO;
	if (game->layout.b3p <= -Y_RES)
		game->layout.b3p = 0;
}

void bird_movement(t_game *game)
{
	if (game->bird.alive)
	{
		game->bird.y += (game->bird.v / (float)FRAME_RATE);
		game->bird.v += ((float)game->phys.bird_ac / (float)FRAME_RATE);
	}
	if (!game->running && game->bird.alive && game->bird.v > game->phys.bird_jump)
		game->bird.v = -game->phys.bird_jump;
	image_to_frame(game, game->bird.img, game->bird.x, game->bird.y);
	if (game->bird.y >= Y_RES)
	{
		game->running = 0;
		game->phys.obs_speed = 0;
		game->bird.alive = 0;
	}
}

void obstacles(t_game *game)
{
	float closest_obs = 0;

	for (int i = 0; i < MAX_OBS; i++)
	{
		if (game->obstacles[i].active)
		{
			if (game->obstacles[i].x > closest_obs) 
				closest_obs = game->obstacles[i].x;
			collision_check(game, i);
			game->obstacles[i].x -= (game->phys.obs_speed / (float)FRAME_RATE);
			score_check(game, i);
			if (game->obstacles[i].x + game->img[OBSTACLE].w < 0)
				game->obstacles[i].active = 0;
			print_obstacles(game, i);
		}
	}
	if (X_RES - closest_obs + game->img[BOTTOM_OBS].w > game->phys.obs_distance || !closest_obs)
		initialize_obstacle(game);
	game->phys.obs_speed += ((float)game->phys.obs_ac / (float)FRAME_RATE);
}
