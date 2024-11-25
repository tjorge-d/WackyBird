#include "WackyBird.h"

static void stop_game(t_game *game)
{
	game->phys.game_speed = 0;
	game->bird.alive = 0;
	game->running = 0;
	game->layout.menu_p = Y_RES;
	game->bird.img = &game->img[BIRDEAD];
}

static void	print_menu(t_game *game, int menu)
{
	image_to_frame(game, game->img[menu], game->layout.menu_x, game->layout.menu_p);
	if (game->layout.menu_p > game->layout.menu_y)
		game->layout.menu_p -= game->layout.menu_speed / (float)FRAME_RATE;
}

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
	if (game->running && (!(game->obstacles[i].x + game->img[BOTTOM_OBS].w <= game->bird.x + game->bird.h * BIRD_HIT || \
	game->obstacles[i].x >= game->bird.x + game->bird.w || \
	game->obstacles[i].bottom_y + game->img[BOTTOM_OBS].h <= game->bird.y || \
	game->obstacles[i].bottom_y >= game->bird.y + game->bird.h) || \
	!(game->obstacles[i].x + game->img[TOP_OBS].w <= game->bird.x + game->bird.h * BIRD_HIT || \
	game->obstacles[i].x >= game->bird.x + game->bird.w || \
	game->obstacles[i].top_y + game->img[TOP_OBS].h <= game->bird.y || \
	game->obstacles[i].top_y >= game->bird.y + game->bird.h)))
	{
		game->bird.v = -game->phys.bird_jump;
		stop_game(game);
	}
}

static void score_check(t_game *game, int i)
{
	if (!game->obstacles[i].collected && game->obstacles[i].x + game->img[TOP_OBS].w / 2 < game->bird.x)
	{
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

static void background(t_game *game)
{
	if (game->bird.alive)
		game->layout.b1p -= (game->phys.game_speed / (float)FRAME_RATE) * (float)B1_SPEED_RATIO;
		game->layout.b2p -= (game->phys.game_speed / (float)FRAME_RATE) * (float)B2_SPEED_RATIO;
		game->layout.b3p -= (game->phys.game_speed / (float)FRAME_RATE) * (float)B3_SPEED_RATIO;
	image_to_frame(game, game->img[B3], game->layout.b3p, 0);
	if (game->layout.b3p <= -Y_RES)
		game->layout.b3p = 0;
	image_to_frame(game, game->img[B2], game->layout.b2p, 0);
	if (game->layout.b2p <= -Y_RES)
		game->layout.b2p = 0;
	image_to_frame(game, game->img[B1], game->layout.b1p, 0);
	if (game->layout.b1p <= -Y_RES)
		game->layout.b1p = 0;
}

static void bird_movement(t_game *game)
{
	game->bird.y += (game->bird.v / (float)FRAME_RATE);
	game->bird.v += ((float)game->phys.bird_ac / (float)FRAME_RATE);
	if (!game->running && game->bird.alive && game->bird.v > game->phys.bird_jump)
		game->bird.v = -game->phys.bird_jump;
	if (!game->bird.alive)
		game->bird.x -= (game->bird.knockback / (float)FRAME_RATE);
	image_to_frame(game, *(game->bird.img), game->bird.x, game->bird.y);
	if ((game->time.tv_sec * 1000000 + game->time.tv_usec) > game->bird.next_flap && game->bird.alive)
	{
		if ((game->bird.img) == &game->img[BIRDOWN])
			game->bird.img = &game->img[BIRDUP];
		else
			game->bird.img = &game->img[BIRDOWN];
		game->bird.next_flap = game->time.tv_sec * 1000000 \
		+ game->time.tv_usec + (1000000 / (FRAME_RATE * FLAP_FREQUENCY) );
	}

	if ((game->bird.y >= Y_RES || game->bird.y + game->bird.h < 0 ) && game->running)
		stop_game(game);
}

static void obstacles(t_game *game)
{
	float closest_obs = 0;

	for (int i = 0; i < MAX_OBS; i++)
	{
		if (game->obstacles[i].active)
		{
			if (game->obstacles[i].x > closest_obs) 
				closest_obs = game->obstacles[i].x;
			collision_check(game, i);
			if (game->bird.alive)
				game->obstacles[i].x -= (game->phys.game_speed / (float)FRAME_RATE);
			score_check(game, i);
			if (game->obstacles[i].x + game->img[BOTTOM_OBS].w < 0)
				game->obstacles[i].active = 0;
			print_obstacles(game, i);
		}
	}
	if ((X_RES - closest_obs + game->img[BOTTOM_OBS].w > game->phys.obs_distance || !closest_obs) && game->running)
		initialize_obstacle(game);
	if (game->bird.alive && game->running)
		game->phys.game_speed += ((float)game->phys.obs_ac / (float)FRAME_RATE);
}

static void print_score(t_game *game, int score)
{
	int iterations = 0;
	while (score > 9)
	{
		image_to_frame(game, game->img[score % 10], \
		game->layout.score_x - (game->img[score % 10].w * game->img[score % 10].scale * iterations), game->layout.score_y);
		iterations++;
		score = score / 10;
	}
	image_to_frame(game, game->img[score % 10], \
	game->layout.score_x - (game->img[score % 10].w * game->img[score % 10].scale * iterations), game->layout.score_y);
}

static void print_ui(t_game *game)
{
	if (game->bird.alive && !game->running)
		print_menu(game, GAMESTART);
	if (!game->bird.alive && !game->running)
		print_menu(game, GAMEOVER);
	if (game->score >= 0)
		print_score(game, game->score);
}

int	game_loop(t_game *game)
{
	gettimeofday(&game->time, NULL);
	if ((game->time.tv_sec * 1000000 + game->time.tv_usec) > game->last_frame)
	{
		background(game);
		bird_movement(game);
		obstacles(game);
		print_ui(game);
		mlx_put_image_to_window(game->mlx, game->window, game->img[FRAME].img, 0, 0);
		gettimeofday(&game->time, NULL);
		game->last_frame = game->time.tv_sec * 1000000 \
		+ game->time.tv_usec + (1000000 / FRAME_RATE);
	}
	return (1);
}
