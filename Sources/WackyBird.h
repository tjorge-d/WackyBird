#ifndef WACKYBIRD_H
# define WACKYBIRD_H

# include "../libft/libft.h"
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
# define X_RES 540
# define Y_RES 960

# define B1_SPEED_RATIO 0.15
# define B2_SPEED_RATIO 0.45
# define B3_SPEED_RATIO 0.80

# define JUMP_STR 300
# define ACCELERATION 600

# define OBS_START_SPEED 60
# define OBS_AC 20000
# define OBS_GAP 200
# define OBS_FREQ 5
# define OBS_INC_RATE 30
# define MAX_OBS 10

# define FRAME_RATE 60
# define ASSET_NUMBER 5
# define IMG_NUMBER 4
# define BACKGROUND_LENGHT 4

typedef enum assets
{
	BIRD,
	BACKGROUND1,
	BACKGROUND2,
	BACKGROUND3,
	OBSTACLE,
	FRAME,
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
	float		y;
	float		v;
}	t_bird;

typedef struct s_obstacles
{
	float			x;
	int 			active;
	float			gap_start;
	float			bottom_y;
	float			bottom_h;
	float			top_y;
	float			top_h;
}	t_obstacles;

typedef struct s_settings
{
	int	w;
	int	h;
	int fps;
}	t_settings;

typedef struct s_layout
{
	float b1s;
	float b1p;
	float b2s;
	float b2p;
	float b3s;
	float b3p;
	long				next_obs_inc;
	float				obs_s;
	int					obs_n;
}	t_layout;

typedef struct s_game
{
	void				*mlx;
	void				*window;
	long				last_frame;
	t_image				*img;
	t_bird				bird;
	t_obstacles			*obstacles;
	t_settings			settings;
	t_layout			layout;
	struct timeval		time;
}	t_game;

unsigned int get_color(t_image *img, int x, int y);
void my_mlx_pixel_force(t_image *img, int x, int y, int color);
void my_mlx_pixel_put(t_image *img, int x, int y, int color);
void image_to_frame(t_game *game, t_image image, int x, int y);

void load_game(t_game *game);

void bird_movement(t_game *game);
void generate_random_gap(t_game *game, int obs_n);
void print_obstacles(t_game *game, int i);
void obstacles(t_game *game);
void background(t_game *game);

int	game_close(t_game *game, int exit_code);

#endif