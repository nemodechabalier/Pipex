CC = cc

NAME = pipex

FLAGS = -Wall -Wextra -Werror -g3

INCLUDES = -I .

SRCS_DIR = srcs/

FUNC = exec.c here_doc.c pipex.c pipex_utils.c redirection.c struct_func.c

SRCS = $(addprefix $(SRCS_DIR), $(FUNC))

OBJS_DIR = obj/

OBJS = $(addprefix $(OBJS_DIR), $(notdir $(SRCS:.c=.o)))

LIBFT = libft_all/libft_printf_gnl.a

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c
	$(MAKE) -C libft_all/
	@mkdir -p $(OBJS_DIR)
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(FLAGS) $(OBJS) $(LIBFT) -o $(NAME)

clean:
	$(MAKE)	-C libft_all/ clean
	rm -rf $(OBJS_DIR)

fclean: clean
	$(MAKE) -C libft_all/ fclean
	rm -f $(NAME) 

re: fclean all

all: $(NAME)

.PHONY: all clean fclean re
