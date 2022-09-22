#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>

#ifdef TEST_SH
# define TEST		1
#else
# define TEST		0
#endif

#define SUCCESS 0
#define FAILURE -1
#define IN 1
#define OUT 0

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
	return (1);
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


int	main(int	ac, char	**av, char	**env)
{
	int	i;
	int	tmp_fd;
	int	pipe_fd[2];
	int	pid;
	(void)	ac;
	(void) env; // a enlever

	i = 0;
	pid = 0;
	tmp_fd = dup(STDIN_FILENO);
	while(av[i] && av[i + 1])
	{
		av = &av[i + 1]; //on avance dans av
		i = 0;
		while (av[i] && strcmp(";", av[i]) != 0 && strcmp("|", av[i]) != 0)
			i++;
		if (strcmp("cd", av[0]) == 0) //CD
			ft_cd(av, i);
		else if (i != 0 && (av[i] == NULL || strcmp(av[i], ";") == 0)) //END LINE
		{
			pid = fork();
			if (pid == 0) //CHILD
			{
				dup2(tmp_fd, STDIN_FILENO); //repetition
				ft_execute(av, i, tmp_fd, env);
				return (1);
			}
			else //PARENT
			{
				close(tmp_fd);
				tmp_fd = dup(STDIN_FILENO);
				waitpid(-1, NULL, WUNTRACED);

			}
		}
		else if (i != 0 && strcmp("|", av[i]) == 0) //PIPE
		{
			pipe(pipe_fd);
			pid = fork();
			if (pid == 0) //CHILD
			{
				dup2(tmp_fd, STDIN_FILENO); //repetition
				dup2(pipe_fd[IN], STDOUT_FILENO);
				close(pipe_fd[IN]);
				close(tmp_fd);
				ft_execute(av, i, tmp_fd, env);
				return (1);
			}
			else //PARENT
			{
				close(pipe_fd[IN]);
				close(tmp_fd);
				tmp_fd = pipe_fd[OUT];
			}
		}
	}
	close(tmp_fd);
	if (TEST) //tester
		while (1);//tester
	return (0);
}
