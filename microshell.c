/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: exam <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 13:02:11 by exam              #+#    #+#             */
/*   Updated: 2023/06/13 14:22:24 by exam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

// /-/ BASIC UTILS /-/ //

int	ft_strlen(char *str)
{
	int	i = 0;

	while (str[i] != '\0')
		i++;
	return (i);
}

void	ft_putstr_fd(char *str, int fd)
{
	int	i = 0;

	while (str[i] != '\0')
	{
		write(fd, &str[i], 1);
		i++;
	}
}

char	*ft_strdup(char *s1)
{
	int		i = 0;
	char	*str;

	str = malloc(sizeof(char) * (ft_strlen(s1) + 1));
	if (!str)
	{
		ft_putstr_fd("error: fatal\n", 2);
		exit(1);
	}
	while (s1[i] != '\0')
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

void	ft_free_fds(int **thing)
{
	int	i = 0;

	while (thing[i] != NULL)
	{
		free(thing[i]);
		i++;
	}
	free(thing);
}

void	ft_free_tab(char **thing)
{
	int	i = 0;

	while (thing[i] != NULL)
	{
		free(thing[i]);
		i++;
	}
	free(thing);
}

// /-/ UTILS /-/ //

void	ft_error(void)
{
	ft_putstr_fd("error: fatal\n", 2);
	exit(1);
}

int	**ft_create_fds(char **str)
{
	int		i = 0;
	int		count = 0;
	int		**fds;

	while (str[i] != NULL && strcmp(str[i], ";") != 0)
	{
		if (str[i][0] == '|')
			count++;
		i++;
	}
	if (count == 0)
		return (NULL);
	else
	{
		fds = malloc(sizeof(int *) * count + 1);
		if (!fds)
			ft_error();
		i = 0;
		while (i != count)
		{
			fds[i] = malloc(sizeof(int) * 2);
			if (!fds[i])
				ft_error();
			i++;
		}
		fds[i] = NULL;
	}
	return (fds);
}

char	**ft_create_args(char **str)
{
	int		i = 0;
	char	**args;

	while (str[i] != NULL && strcmp(str[i], "|") != 0 && strcmp(str[i], ";") != 0)
		i++;
	args = malloc(sizeof(char *) * i + 1);
	if (!args)
		ft_error();
	i = 0;
	while (str[i] != NULL && strcmp(str[i], "|") != 0 && strcmp(str[i], ";") != 0)
	{
		args[i] = ft_strdup(str[i]);
		if (!args[i])
			ft_error();
		i++;
	}
	args[i] = NULL;
	return (args);
}

void	ft_cd(char **args)
{
	int	i = 0;

	while (args[i] != NULL && strcmp(args[i], ";") != 0)
		i++;
	if (i != 2)
		ft_putstr_fd("error: cd: bad arguments\n", 2);
	else
	{
		i = chdir(args[1]);
		if (i == -1)
		{
			ft_putstr_fd("error: cd: cannot change directory to ", 2);
			ft_putstr_fd(args[1], 2);
			ft_putstr_fd("\n", 2);
		}
	}
}

void	ft_exec_cmd(char **args, char **envp)
{
	int		i = 0;
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		i = execve(args[0], args, envp);
		if (i == -1)
		{
			ft_putstr_fd("error: cannot execute ", 2);
			ft_putstr_fd(args[0], 2);
			ft_putstr_fd("\n", 2);
			exit(1);
		}
	}
}

void	ft_execute(char **args, char **envp)
{
	int		i = 0;
	int		j = 0;
	int		saved_stdout;
	int		**fds;
	char	**new_args;

	if (strcmp(args[0], "cd") == 0)
	{
		new_args = ft_create_args(args);
		ft_cd(new_args);
		ft_free_tab(new_args);
	}
	else
	{
		fds = ft_create_fds(args);
		if (fds == NULL)
		{
			new_args = ft_create_args(args);
			ft_exec_cmd(new_args, envp);
			ft_free_tab(new_args);
		}
		else
		{
			saved_stdout = dup(1);
			while (fds[i] != NULL)
			{
				if (pipe(fds[i]) == -1)
					ft_error();
				if (i != 0)
					dup2(fds[i - 1][0], 0), close(fds[i - 1][0]);
				dup2(fds[i][1], 1), close(fds[i][1]);
				new_args = ft_create_args(args + j);
				ft_exec_cmd(new_args, envp);
				ft_free_tab(new_args);
				while (args[j] != NULL && strcmp(args[j], "|") != 0 && strcmp(args[j], ";") != 0)
					j++;
				j = j + 1;
				i++;
			}
			dup2(fds[i - 1][0], 0), close(fds[i - 1][0]);
			dup2(saved_stdout, 1);
			new_args = ft_create_args(args + j);
			ft_exec_cmd(new_args, envp);
			ft_free_tab(new_args);
			close(saved_stdout);
			ft_free_fds(fds);
		}
		while (waitpid(-1, NULL, WUNTRACED) != -1)
			;
	}
}

int main(int argc, char **argv, char **envp)
{
	int	i = 1;

	if (argc <= 1)
		return (0);
	while (argv[i] != NULL)
	{
		while (argv[i] != NULL && strcmp(argv[i], ";") == 0)
			i++;
		if (argv[i] != NULL)
			ft_execute(argv + i, envp);
		while (argv[i] != NULL && strcmp(argv[i], ";") != 0)
			i++;
	}
	return (0);
}
