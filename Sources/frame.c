#include "WackyBird.h"

unsigned int get_color(t_image *img, int x, int y)
{
	char	*dst;

	dst = img->addr + (y * img->line_length + x * (img->bpp / 8));
	return (*(unsigned int *)dst);
}

void my_mlx_pixel_force(t_image *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_length + x * (img->bpp / 8));
	*(unsigned int *)dst = color;
}


void my_mlx_pixel_put(t_image *img, int x, int y, int color)
{
	char	*dst;

	if (color == TRANSPARENT)
		return ;

	dst = img->addr + (y * img->line_length + x * (img->bpp / 8));
	*(unsigned int *)dst = color;
}

void image_to_frame(t_game *game, t_image image, int x, int y)
{
	int		x_off = 0;
	int		y_off = 0;

	if (x < 0)
		x_off = -x;
	if (y < 0)
		y_off = -y;
	for (int i = y_off; i < ((float)image.h * image.scale) && y + i < Y_RES; i++)
	{
		for (int j = x_off; j < ((float)image.w * image.scale) && x + j < X_RES; j++)
		{
			my_mlx_pixel_put(&game->img[FRAME], x + j, y + i, \
			get_color(&image, (float)j / image.scale, (float)i / image.scale));
		}
	}
}
