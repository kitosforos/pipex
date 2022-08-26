/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maralons <maralons@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 15:58:24 by maralons          #+#    #+#             */
/*   Updated: 2022/08/26 20:14:04 by maralons         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

char	*check_path(char **envp, char **cmd)
{
	int		i;
	char	**paths;
	char	*path;
	char	*path2;

	i = 0;
	while (!ft_strnstr(envp[i], "PATH", 4))
		i++;
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (i < PATHS)
	{
		path = ft_strjoin(paths[i], "/");
		path2 = ft_strjoin(path, cmd[0]);
		free(path);
		if (!access(path2, F_OK))
			return (path2);
		else
			free(path2);
		i++;
	}
	i = -1;
	while (paths[++i])
		free(paths[i]);
	free(paths);
	return(0);
}

void	execute(char *argv, char *envp[])
{
	char	**cmd;
	char	*path;
	int	i;

	i = -1;
	cmd = ft_split(argv, ' ');
	path = check_path(envp, cmd);
	if (!path)
	{
		while (cmd[++i])
			free(cmd[i]);
		free(cmd);
		perror("Error");
		exit(EXIT_FAILURE);
	}
	if (execve(path, cmd, envp) == -1)
	{
		perror("Error");
		exit(EXIT_FAILURE);
	}
	free(cmd);
}

void	child_process(int *pip, char *argv, char **envp)
{
	close(pip[READ_END]);
	dup2(pip[WRITE_END], STDOUT_FILENO);
	close(pip[WRITE_END]);
	execute(argv, envp);
}

void	parent_process(int *pip, char *argv, char **envp, int fd2)
{
	close(pip[WRITE_END]);
	dup2(pip[READ_END], STDIN_FILENO);
	close(pip[READ_END]);
	dup2(fd2, STDOUT_FILENO);
	close(fd2);
	execute(argv, envp);
}

int	main(int argc, char *argv[], char *envp[])
{
	int		fd;
	int		fd2;
	int		pip[2];
	pid_t	p;

	if (argc != 5)
	{
		write(1, "Not 4 arguments\n", 16);
		return (1);
	}
	pipe(pip);
	fd = open(argv[1], O_RDONLY);
	dup2(fd, STDIN_FILENO);
	close(fd);
	p = fork();
	if (p == -1)
		perror("Error in fork");
	else if (p == 0)
		child_process(pip, argv[2], envp);
	else
	{
		waitpid(p, NULL, 0);
		fd2 = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
		parent_process(pip, argv[3], envp, fd2);
	}
}
