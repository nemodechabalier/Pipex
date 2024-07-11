#ifndef PIPEX_H
# define PIPEX_H

# include "../libft_all/ft_printf/ft_printf.h"
# include "../libft_all/get_next_line/get_next_line.h"
# include "../libft_all/libtf/libft.h"
# include <fcntl.h>
# include <sys/wait.h>

# ifndef TRUE
#  define TRUE 1
# endif

# ifndef FALSE
#  define FALSE 0
# endif

typedef struct s_pip
{
	int				pipe_fd[2];
	char			**paths;
	char			**envp;
	struct s_pip	*next;
	struct s_pip	*prev;
}					t_pip;

// creat cmd && error
int					error_files(char *file_1, char *file_2);
char				**find_paths(char **envp);
char				*find_path(char **paths, char *cmd);
void				exec_child_last(t_pip *pip, char *cmd, char *file);
void				exec_child_first(t_pip *pip, char *cmd, char *file);
int					pipe_to_file(char *file, t_pip *pip);
int					file_to_pipe(char *file, t_pip *pip);
void				pipe_to_pipe(int pipe_fd_1[2], int pipe_fd_2[2],
						t_pip *pip);
void				exec_child_midle(t_pip *pip, char *cmd);

// utils
void				ft_free_strs(char **strs);

// do cmd
void				exec(pid_t pid, char **cmd, char **envp);

// struct utils
int					pip_add_back(t_pip **pip, t_pip *new);
t_pip				*pip_new(char **envp);
void				del_pip(t_pip **pip);
t_pip				*last_pip(t_pip *pip);
void				close_pipe(t_pip *pip);

#endif