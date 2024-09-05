#include "microshell.h"

void	ft_error(void)
{
	ft_putstr_fd("error: fatal\n", 2);
	exit(1);
}

int main(const int argc, const char **argv, char **envp)
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
