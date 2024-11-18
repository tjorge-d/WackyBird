#ifndef WACKYBIRD_H
# define WACKYBIRD_H

# include "../.mlx/mlx.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <time.h>
# include <math.h>
# include <sys/time.h>
# include <sys/types.h> 
# include <sys/wait.h>
# include <sys/stat.h>
# include <errno.h>
# include <signal.h>
# include <limits.h>

# define TRANSPARENT 16777215
# define Y_RES 800
# define X_RES 600
# define FRAME_RATE 60
# define ASSET_NUMBER 18
# define IMG_NUMBER 6
# define BACKGROUND_LENGHT 10

// UI
# define B1_SPEED_RATIO 0.80
# define B2_SPEED_RATIO 0.45
# define B3_SPEED_RATIO 0.15
# define MENU_SPEED 1
# define SCORE_SIZE 0.1
# define SCORE_POS 0.03

// BIRD PROPERTIES
# define JUMP_STR 0.7
# define ACCELERATION 2
# define BIRD_X 0.3
# define BIRD_SIZE 0.1
# define BIRD_KNOCKBACK 0.1

// OBSTACLES PROPERTIES
# define OBS_SPEED 0.1
# define OBS_GAP 0.3
# define OBS_DISTANCE 0.4
# define OBS_WIDTH 0.15
# define OBS_HEIGHT 10
# define MAX_OBS 10


typedef enum assets
{
	ZERO,
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	BIRD,
	OBSTACLE,
	OBSTACLE_END,
	GAMESTART,
	GAMEOVER,
	BACKGROUND1,
	BACKGROUND2,
	BACKGROUND3,
	FRAME,
	TOP_OBS,
	BOTTOM_OBS,
	B1,
	B2,
	B3
}	t_assets;

typedef enum type
{
	FORE = 119,
	ESC = 65307,
}	t_type;

typedef struct s_image
{
	void	*img;
	char	*addr;
	char	*path;
	int		bpp;
	int		line_length;
	int		endian;
	int		h;
	int		w;
	float	scale;
	int		created;
}	t_image;

typedef struct s_bird
{
	t_image		img;
	int			alive;
	int			h;
	int			w;
	float		x;
	float		y;
	float		v;
	float		knockback;
}	t_bird;

typedef struct s_obstacles
{
	float			x;
	int 			active;
	int				collected;
	float			gap_start;
	float			bottom_y;
	float			top_y;
}	t_obstacles;

typedef struct s_phys
{
	float		bird_ac;
	float		bird_jump;
	float		game_speed;
	float		obs_ac;
	float		obs_distance;
	float		obs_gap;
}	t_phys;

typedef struct s_layout
{
	float	b1s;
	float	b1p;
	float	b2s;
	float	b2p;
	float	b3s;
	float	b3p;
	float	menu_p;
	float	menu_y;
	float	menu_x;
	float	menu_speed;
	float	score_y;
	float	score_x;
}	t_layout;

typedef struct s_game
{
	void				*mlx;
	void				*window;
	long				last_frame;
	int					score;
	int					running;
	t_image				*img;
	t_bird				bird;
	t_obstacles			*obstacles;
	t_phys				phys;
	t_layout			layout;
	struct timeval		time;
}	t_game;

unsigned int get_color(t_image *img, int x, int y);
void my_mlx_pixel_force(t_image *img, int x, int y, int color);
void my_mlx_pixel_put(t_image *img, int x, int y, int color);
void image_to_frame(t_game *game, t_image image, int x, int y);

void load_game(t_game *game);

int	game_loop(t_game *game);

int	game_close(t_game *game, int exit_code);

#endif