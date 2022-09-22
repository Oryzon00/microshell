#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>

#define SUCCESS 0
#define FAILURE -1

int	ft_putstr_err(char	*str, char	*arg)
{
	int	i;

	i = 0;
	while(str[i])
	{
		write(STDERR_FILENO, &str[i], 1);
		i++;
	}
	i = 0;
	if (arg != NULL)
	{
		while(arg[i])
		{
			write(STDERR_FILENO, &arg[i], 1);
			i++;
		}
	}
	write(STDERR_FILENO, "\n", 1);
	return (0);
}

int	ft_execute(char **av, int i, int tmp_fd, char **env)
{
	av[i] = NULL; //overwrite ";" "|" NULL par NULL pour execve
	dup2(tmp_fd, STDIN_FILENO);
	close(tmp_fd);
	execve(av[0], av, env);
	ft_putstr_err("error: cannot execute ", av[0]); //ne pas oublier espace
	return (0);
}

int	ft_cd(char **av, int i)
{
	if (i != 2)
		ft_putstr_err("error: cd: bad arguments", NULL);
	else
	{
		if (chdir(av[1]) != SUCCESS)
			ft_putstr_err("error: cd: cannot change directory to ", av[1]);
	}
	return (0);
}

int	ft_end_line()
{
	return (0);
}

int	ft_pipe()
{
	return (0);
}

int	main(int	ac, char	**av, char	**env)
{
	int	i;
	int	tmp_fd;
	(void)	ac;
	(void) env; // a enlever

	i = 0;
	tmp_fd = dup(STDIN_FILENO);
	while(av[i] && av[i + 1])
	{
		av = &av[i + 1]; //on avance dans av
		i = 0;
		while (av[i] && strcmp(";", av[i]) != 0 && strcmp("|", av[i]) != 0)
			i++;
		if (strcmp("cd", av[0]) == 0)
			ft_cd(av, i);
		else if (i != 0 && (av[i] == NULL || strcmp(av[i], ";") == 0))
			ft_end_line();
		else if (i != 0 && strcmp("|", av[i]) == 0)
			ft_pipe();
	}
	close(tmp_fd);
}
