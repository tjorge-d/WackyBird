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
# define X_RES 600
# define Y_RES 800
# define FRAME_RATE 60
# define ASSET_NUMBER 6
# define IMG_NUMBER 6
# define BACKGROUND_LENGHT 10

// BACKGROUND
# define B1_SPEED_RATIO 0.15
# define B2_SPEED_RATIO 0.45
# define B3_SPEED_RATIO 0.80

// BIRD PROPERTIES
# define JUMP_STR 0.7
# define ACCELERATION 2
# define BIRD_X 0.3
# define BIRD_SIZE 0.005

// OBSTACLES PROPERTIES
# define OBS_SPEED 0.1
# define OBS_GAP 0.3
# define OBS_DISTANCE 0.4
# define OBS_WIDTH 0.15
# define OBS_HEIGHT 10
# define MAX_OBS 10


typedef enum assets
{
	BIRD,
	BACKGROUND1,
	BACKGROUND2,
	BACKGROUND3,
	OBSTACLE,
	OBSTACLE_END,
	FRAME,
	B1,
	B2,
	B3,
	TOP_OBS,
	BOTTOM_OBS
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
	float		obs_speed;
	float		obs_ac;
	float		obs_distance;
	float		obs_gap;
}	t_phys;

typedef struct s_layout
{
	float b1s;
	float b1p;
	float b2s;
	float b2p;
	float b3s;
	float b3p;
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

void background(t_game *game);
void bird_movement(t_game *game);
void obstacles(t_game *game);

int	game_close(t_game *game, int exit_code);

#endif