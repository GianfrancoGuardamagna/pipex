#include "pipex.h"

static char	*get_path_from_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (envp[i][0] == 'P' && envp[i][1] == 'A' && \
envp[i][2] == 'T' && envp[i][3] == 'H' && envp[i][4] == '=')
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

static void execute_process(char* args, int *pipefd, int type, char **envp)
{
	char	**env_path;
	char	*bin_path;
    char    *env;
    int         i;

    env = get_path_from_env(envp);
	env_path = ft_split(env, ':');
	bin_path = find_binary(args, env_path);
	if(type == 1)
		dup2(pipefd[1], STDOUT_FILENO);
	else
		dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
    i = 0;
    while(env_path[i])
    {
        free(env_path[i]);
        i++;
    }
	if(execve(bin_path, args, envp) == -1)
		exit((perror("execve"), 127));
}

static int	process_loader(char **argv, char **envp, int *fd, int child)
{
		char	*args;
		int	file;

		args = NULL;
		if(child == 1)
		{
			args = argv[2];
			file = open(argv[1], O_RDONLY);
			if (file < 0)
				return(failed_fd(args));
			dup2(file, STDIN_FILENO);
			dup2(fd[1], STDOUT_FILENO);
		}
		else
		{
			args = argv[4];
			file = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (file < 0)
				return(failed_fd(args));
			dup2(fd[0], STDIN_FILENO);
			dup2(file, STDOUT_FILENO);
		}
		execute_process(args, fd, child, envp);
		return(0);
}

int	main(int argc, char **argv, char **envp)
{
	int			fd[2];
	pid_t		pid1;
	pid_t		pid2;

	if (argc != 5)
		return (write(2, "Invalid arguments, please try again:\n\
./pipex infile cmd1 cmd2 outfile\n", 70));
	if (pipe(fd) == -1)
		return (1);
	pid1 = fork();
	if (pid1 < 0)
		return (1);
	if (pid1 == 0)
		process_loader(argv, envp, fd, 1);
	pid2 = fork();
	if (pid2 < 0)
		return (1);
	if (pid2 == 0)
		process_loader(argv, envp, fd, 2);
	freeing_memory(fd, pid1, pid2);
	return (0);
}
