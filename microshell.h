#ifndef MICROSHELL_H
# define MICROSHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>

# include <sys/types.h>
# include <sys/wait.h>

# include <fcntl.h>

int     main(const int argc, const char **argv, char **envp);
void    ft_error(void);

int		**ft_create_fds(const char **str);
char	**ft_create_args(const char **str);
void	ft_exec_cmd(char **args, char **envp);
void	ft_execute(const char **args, char **envp);

void	ft_cd(char **args);

void	ft_free_fds(int **thing);
void	ft_free_tab(char **thing);

int	    ft_strlen(const char *str);
void	ft_putstr_fd(const char *str, const int fd);
char	*ft_strdup(const char *s1);

#endif
