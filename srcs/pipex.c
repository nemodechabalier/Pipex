/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nde-chab <nde-chab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 15:47:35 by nde-chab          #+#    #+#             */
/*   Updated: 2024/07/11 17:40:36 by nde-chab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	take_path_exec(char **av, char **envp)
{
	t_pip	*pip;
	int		i;

	i = 4;
	pip = NULL;
	if (pip_add_back(&pip, pip_new(envp)) == -1)
		return ;
	if (pipe(pip->pipe_fd) == -1)
		return (perror("pip"));
	exec_child_first(pip, av[2], av[1]);
	while (av[i + 1])
	{
		if (pip_add_back(&pip, pip_new(envp)) == -1)
			return ;
		pip = last_pip(pip);
		if (pipe(pip->pipe_fd) == -1)
			return (perror("pip"));
		exec_child_midle(pip, av[i - 1]);
		close(pip->prev->pipe_fd[0]);
		close(pip->prev->pipe_fd[1]);
		i++;
	}
	exec_child_last(pip, av[i - 1], av[i]);
	close(pip->pipe_fd[0]);
	close(pip->pipe_fd[1]);
	 while (i > 2)
	{
		wait(NULL);
		i--;
	}
	del_pip(&pip);
}

int	main(int ac, char **av, char **envp)
{
	if (ac == 1)
		return (ft_printf("need 4 args : file1 cmd1 cmd2 file2"), -1);
	else
	{
		take_path_exec(av, envp);
	}
	return (0);
}
