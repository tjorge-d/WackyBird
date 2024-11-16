NAME= WackyBird

CC= gcc
CFLAGS= -Ofast -g

SRC= $(addprefix Sources/, $(SOURCES))
SOURCES= main.c loading.c loop.c frame.c
		
OBJ_DIR= objects
OBJ= $(addprefix $(OBJ_DIR)/, $(SRC:Sources/%.c=%.o)) \

MLX_DIR= ./.mlx
MLX= $(MLX_DIR)/libmlx_Linux.a -lXext -lX11 -lm -lz

all: $(NAME)

$(NAME): $(OBJ) $(MLX)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(MLX)

$(OBJ_DIR)/%.o: Sources/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(MLX):
	$(MAKE) -C $(MLX_DIR)

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(MLX_DIR) clean

fclean: clean
	rm -f $(NAME)

re: fclean all

run: re 
	clear && ./WackyBird

.PHONY: all fclean clean re run