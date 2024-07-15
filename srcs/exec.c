/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nde-chab <nde-chab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 15:43:11 by nde-chab          #+#    #+#             */
/*   Updated: 2024/07/12 17:12:10 by nde-chab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	ft_exec(char *path, char **cmds, t_pip *pip)
{
	if (path[0] == '0' && path[1] == 0 && (ft_strncmp(pip->paths[0], "0",
				2) != 0))
	{
		ft_putstr_fd(cmds[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		free(path);
		ft_free_strs(cmds);
		del_pip(&pip);
		exit(EXIT_FAILURE);
		return ;
	}
	if (execve(path, cmds, pip->envp) == -1)
	{
		perror(cmds[0]);
		free(path);
		ft_free_strs(cmds);
		del_pip(&pip);
		exit(EXIT_FAILURE);
		return ;
	}
	exit(EXIT_FAILURE);
}

void	exec_child_last(t_pip *pip, char *cmd, char *file)
{
	pid_t	pid;
	char	*path;
	char	**cmds;

	cmds = ft_split(cmd, ' ');
	path = find_path(pip->paths, cmds[0]);
	if (!cmds || !path)
		return (ft_free_strs(cmds), free(path), del_pip(&pip));
	pid = fork();
	if (pid == -1)
		return (ft_free_strs(cmds), free(path), del_pip(&pip));
	else if (pid == 0)
	{
		if (pipe_to_file(file, pip) == 1)
			return (ft_exec(path, cmds, pip));
		else
			return (free(path), ft_free_strs(cmds), del_pip(&pip),
				exit(EXIT_FAILURE));
	}
	else
		return (ft_free_strs(cmds), free(path));
}

void	exec_child_last_here_doc(t_pip *pip, char *cmd, char *file)
{
	pid_t	pid;
	char	*path;
	char	**cmds;

	cmds = ft_split(cmd, ' ');
	path = find_path(pip->paths, cmds[0]);
	if (!cmds || !path)
		return (ft_free_strs(cmds), free(path), del_pip(&pip));
	pid = fork();
	if (pid == -1)
		return (ft_free_strs(cmds), free(path), del_pip(&pip));
	else if (pid == 0)
	{
		if (pipe_to_file_for_here_doc(file, pip) == 1)
			return (ft_exec(path, cmds, pip));
		else
			return (free(path), ft_free_strs(cmds), del_pip(&pip),
				exit(EXIT_FAILURE));
	}
	else
		return (ft_free_strs(cmds), free(path));
}

void	exec_child_first(t_pip *pip, char *cmd, char *file)
{
	pid_t	pid;
	char	*path;
	char	**cmds;

	cmds = ft_split(cmd, ' ');
	path = find_path(pip->paths, cmds[0]);
	if (!cmds || !path)
		return (ft_free_strs(cmds), free(path), del_pip(&pip));
	pid = fork();
	if (pid == -1)
		return (ft_free_strs(cmds), free(path), del_pip(&pip));
	else if (pid == 0)
	{
		if (file_to_pipe(file, pip) == 1)
			return (ft_exec(path, cmds, pip));
		else
			return (free(path), ft_free_strs(cmds), del_pip(&pip),
				exit(EXIT_FAILURE));
	}
	else
		return (ft_free_strs(cmds), free(path));
}

void	exec_child_midle(t_pip *pip, char *cmd)
{
	pid_t	pid;
	char	*path;
	char	**cmds;

	cmds = ft_split(cmd, ' ');
	path = find_path(pip->paths, cmds[0]);
	if (!cmds || !path)
		return (ft_free_strs(cmds), free(path), del_pip(&pip));
	pid = fork();
	if (pid == -1)
		return (ft_free_strs(cmds), free(path), del_pip(&pip));
	else if (pid == 0)
	{
		pipe_to_pipe(pip->prev->pipe_fd, pip->pipe_fd, pip);
		return (ft_exec(path, cmds, pip));
	}
	else
		return (ft_free_strs(cmds), free(path));
}
