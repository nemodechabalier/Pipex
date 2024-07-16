/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nde-chab <nde-chab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 15:36:50 by nde-chab          #+#    #+#             */
/*   Updated: 2024/07/16 17:26:37 by nde-chab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*creat_here_doc(int *file_fd)
{
	char	*name;
	char	*temp;
	int		i;

	i = 0;
	name = ft_strdup("here_doc");
	if (!name)
		return (NULL);
	while (access(name, F_OK) != -1)
	{
		temp = ft_strdup(name);
		free(name);
		if (!temp)
			return (NULL);
		name = ft_strjoin(temp, ft_itoa(i));
		free(temp);
		if (!name)
			return (NULL);
		i++;
	}
	*file_fd = open(name, O_CREAT, 0644);
	close(*file_fd);
	return (name);
}

static void	take_path_exec_here_doc(char **av, char **envp, char *name)
{
	t_pip	*pip;
	int		i;

	i = 5;
	pip = NULL;
	if (pip_add_back(&pip, pip_new(envp)) == -1)
		return ;
	if (pipe(pip->pipe_fd) == -1)
		return (perror("pip"));
	exec_child_first(pip, av[3], name);
	while (av[i + 1])
		i = middle_cmd(&pip, i, envp, av);
	exec_child_last_here_doc(pip, av[i - 1], av[i]);
	close(pip->pipe_fd[0]);
	close(pip->pipe_fd[1]);
	while (i > 3)
	{
		wait(NULL);
		i--;
	}
	del_pip(&pip);
}

void	creat_here_doc_take_path_exec(char **av, char **envp)
{
	char	*name;
	char	*line;
	int		file_fd;

	write(2, "> ", 2);
	name = creat_here_doc(&file_fd);
	if (!name)
		return ;
	line = get_next_line(0);
	file_fd = open(name, W_OK, R_OK);
	while (line)
	{
		if (ft_strncmp(line, av[2], strlen(av[2])) == 0
			&& line[strlen(av[2])] == '\n')
			break ;
		write(2, "> ", 2);
		write(file_fd, line, ft_strlen(line));
		free(line);
		line = get_next_line(0);
	}
	free(line);
	close(file_fd);
	take_path_exec_here_doc(av, envp, name);
	unlink(name);
	free(name);
}
