/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nde-chab <nde-chab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 15:58:07 by nde-chab          #+#    #+#             */
/*   Updated: 2024/07/15 16:24:49 by nde-chab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*find_path(char **paths, char *cmd)
{
	int		i;
	char	*road;
	char	*temp;

	i = 0;
	if (!cmd)
		return (NULL);
	if (access(cmd, F_OK) == 0)
		return (strdup(cmd));
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return (NULL);
		road = ft_strjoin(temp, cmd);
		free(temp);
		if (!road)
			return (NULL);
		if (access(road, F_OK) == 0)
			return (road);
		else
			free(road);
		i++;
	}
	return (ft_strdup("0"));
}

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
		return (ft_split(strdup("0"), ' '));
	paths = ft_split(envp[i] + 5, ':');
	return (paths);
}
