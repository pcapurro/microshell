#include "../include/microshell.h"

int	ft_strlen(const char *str)
{
	int	i = 0;

	while (str[i] != '\0')
		i++;
	return (i);
}

void	ft_putstr_fd(const char *str, const int fd)
{
	int	i = 0;

	while (str[i] != '\0')
	{
		write(fd, &str[i], 1);
		i++;
	}
}

char	*ft_strdup(const char *s1)
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
