/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_func.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nde-chab <nde-chab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:45:31 by nde-chab          #+#    #+#             */
/*   Updated: 2024/07/11 17:09:19 by nde-chab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

t_pip	*pip_new(char **envp)
{
	t_pip	*new;

	new = (t_pip *)malloc(sizeof(t_pip));
	if (!new)
		return (NULL);
	new->envp = envp;
	new->next = NULL;
	new->prev = NULL;
	new->paths = find_paths(envp);
	if (!new->paths)
		return (free(new), NULL);
	return (new);
}

int	pip_add_back(t_pip **pip, t_pip *new)
{
	t_pip	*temp;

	if (!new)
		return (0);
	temp = *pip;
	if (!temp)
	{
		*pip = new;
		return (1);
	}
	while (temp->next)
		temp = temp->next;
	temp->next = new;
	new->prev = temp;
	return (1);
}

void	del_pip(t_pip **pip)
{
	t_pip	*temp;
	t_pip	*save;

	temp = *pip;
	if (!temp)
		return ;
	while (temp->prev)
		temp = temp->prev;
	while (temp)
	{
		save = temp->next;
		ft_free_strs(temp->paths);
		free(temp);
		temp = save;
	}
	*pip = NULL;
}

void	close_pipe(t_pip *pip)
{
	if (!pip)
		return ;
	while (pip->prev)
		pip  =  pip->prev;
	while (pip)
	{
		close(pip->pipe_fd[0]);
		close(pip->pipe_fd[1]);
		pip = pip->next;
	}
}

t_pip	*last_pip(t_pip *pip)
{
	while (pip->next)
		pip = pip->next;
	return (pip);
}