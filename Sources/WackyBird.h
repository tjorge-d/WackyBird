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
# define SIZE 64
# define X_RES 1900
# define Y_RES 1060
# define H_SPEED 0.05
# define JUMP_STR 100 
# define M_PI 3.14159265358979323846
# define FRAME_RATE 60
# define ASSET_NUMBER 5
# define IMG_NUMBER 4
# define BACKGROUND_LENGHT 5

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
	float 		a;
}	t_bird;

typedef struct s_obstacles
{
	float				x;
	double				x_in_texture;
	double				y_stepper;
	double				x_coordinate;
	double				height;
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
}	t_layout;

typedef struct s_game
{
	void				*mlx;
	void				*window;
	long				last_frame;
	t_image				*img;
	t_bird				bird;
	t_obstacles			*obstacles;
	float				ob_s;
	float				ob_a;
	t_settings			settings;
	t_layout			layout;
	struct timeval		time;
}	t_game;

#endif