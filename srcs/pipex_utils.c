/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nde-chab <nde-chab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 15:58:07 by nde-chab          #+#    #+#             */
/*   Updated: 2024/07/11 17:44:43 by nde-chab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_free_strs(char **strs)
{
	int	i;

	i = 0;
	if (!strs)
		return ;
	while (strs[i])
	{
		free(strs[i]);
		strs[i] = NULL;
		i++;
	}
	free(strs);
	strs = NULL;
}

char	**find_paths(char **envp)
{
	int		i;
	char	**paths;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			break ;
		i++;
	}
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	return (paths);
}

void	ft_exec(char *path, char **cmds, t_pip *pip)
{
	if (path[0] == '0' && path[1] == 0)
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

int	file_to_pipe(char *file, t_pip *pip)
{
	int	file_fd;

	file_fd = open(file, O_RDONLY);
	if (file_fd == -1)
		return (perror(file), close_pipe(pip), close(file_fd), 0);
	dup2(file_fd, STDIN_FILENO);
	dup2(pip->pipe_fd[1], STDOUT_FILENO);
	close_pipe(pip);
	close(file_fd);
	return (1);
}

void	pipe_to_pipe(int pipe_fd_1[2], int pipe_fd_2[2], t_pip *pip)
{
	dup2(pipe_fd_1[0], STDIN_FILENO);
	dup2(pipe_fd_2[1], STDOUT_FILENO);
	close_pipe(pip);
}

int	pipe_to_file(char *file, t_pip *pip)
{
	int	file_fd;

	file_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file_fd == -1)
		return (perror(file), close(file_fd), close_pipe(pip), 0);
	dup2(pip->pipe_fd[0], STDIN_FILENO);
	dup2(file_fd, STDOUT_FILENO);
	close_pipe(pip);
	close(file_fd);
	return (1);
}
