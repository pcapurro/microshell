#include "../include/microshell.h"

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
