/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nde-chab <nde-chab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 15:47:35 by nde-chab          #+#    #+#             */
/*   Updated: 2024/07/15 16:31:03 by nde-chab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	middle_cmd(t_pip **pip, int i, char **envp, char **av)
{
	if (pip_add_back(pip, pip_new(envp)) == -1)
		return (-1);
	*pip = last_pip(*pip);
	if (pipe((*pip)->pipe_fd) == -1)
		return (perror("pip2"), -1);
	exec_child_midle(*pip, av[i - 1]);
	close((*pip)->prev->pipe_fd[0]);
	close((*pip)->prev->pipe_fd[1]);
	i++;
	return (i);
}

void	take_path_exec(char **av, char **envp)
{
	t_pip	*pip;
	int		i;

	i = 4;
	pip = NULL;
	if (pip_add_back(&pip, pip_new(envp)) == -1)
		return ;
	if (pipe(pip->pipe_fd) == -1)
		return (perror("pip2"));
	exec_child_first(pip, av[2], av[1]);
	while (av[i + 1])
	{
		i = middle_cmd(&pip, i, envp, av);
		if (i == -1)
			return (del_pip(&pip));
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
	if (ac < 5)
		return (ft_printf("need 4 args : file1 cmd1 cmd2 file2"), -1);
	else
	{
		if (ft_strncmp(av[1], "here_doc", 9) == 0 && ac > 5)
			creat_here_doc_take_path_exec(av, envp);
		else if (ft_strncmp(av[1], "here_doc", 9) != 0)
			take_path_exec(av, envp);
		else
			return (ft_printf("Error whith here_doc\n"), -1);
	}
	return (0);
}
