#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#define SUCCESS 0
#define FAILURE -1
#define IN 1
#define OUT 0

int	ft_putstr_err(char *str, char *arg)
{
	int	i;

	i = 0;
	while(str[i])
	{
		write(STDERR_FILENO, &str[i], 1);
		i++;
	}
	i = 0;
	if (arg[i])
	{
		while(arg[i])
		{
			write(STDERR_FILENO, &arg[i], 1);
			i++;
		}
	}
	write(STDERR_FILENO, "\n", 1);
}

int ft_execute(char **av, int i, int tmp_fd, char **env)
{
	av[i] = NULL;
	dup2(tmp_fd, STDIN_FILENO);
	close(tmp_fd);
	execve(av[0], av, env);
	ft_putstr_err("error: cannot execute ", av[0]);
	return (1);
}

int	main(int ac, char **av, char **env)
{
	int	i;
	int	fd[2];
	(void) ac;
	int	tmp_fd;

	i = 0;
	tmp_fd = dup(STDIN_FILENO);
	while (av[i] && av[i + 1])
	{
		av = &av[i + 1];
		i = 0;
		while (av[i] && strcmp(av[i],";") != 0 && strcmp(av[i], "|") != 0)
			i++;
		if (strcmp(av[0], "cd") == 0)
		{
			if (i != 2)
				ft_putstr_err("error: cd: bad arguments", NULL);
			else if (chdir(av[1]) != SUCCESS)
				ft_putstr_err("error: cd: cannot change directory to ", av[1]);
		}
		else if (i != 0 && (av[i] == NULL || strcmp(av[i], ";") == 0))
		{

		}
		else if (i != 0 && strcmp(av[i], "|"))
		{

		}
	}
	close(tmp_fd);
}
