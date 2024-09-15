#include "../include/microshell.h"

int	**ft_create_fds(const char **str)
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

char	**ft_create_args(const char **str)
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

void	ft_execute(const char **args, char **envp)
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
