#include "../include/microshell.h"

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
