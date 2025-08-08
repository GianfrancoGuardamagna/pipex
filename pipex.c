#include "pipex.h"

void freeing_memory(int *fd, pid_t pid1, pid_t pid2)
{
	close(fd[0]);
    close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}

char *find_binary(char *command, char **paths)
{
    char	*full_path;
	int		i;
    char    *temp;

	i = 1;
	while(paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
        full_path = ft_strjoin(temp, command);
        free(temp);
		if(access(full_path, X_OK) == 0)
			return full_path;
		i++;
	}
	return NULL;
}

void execute_process(char** args, int *pipefd, int type)
{
	char	**env_path;
	char	*bin_path;
    int    i;

	env_path = ft_split(getenv("PATH"), ':');
	bin_path = find_binary(args[0], env_path);
	if(type == 0)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
	}
	else
	{
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
	}
    i = 0;
    while(env_path[i])
    {
        free(env_path[i]);
        i++;
    }
	execve(bin_path, args, env_path);
}

char **get_args(int type, char **argv)
{
	char **args;

	if(type == 0)
	{
		args = ft_split(argv[2], ' ');
	}
	else
	{
		args = ft_split(argv[3], ' ');
	}

    return args;
}

void infile(char *file, int type)
{
    int fd;

    if (type == 0)
    {
        fd = open(file, O_RDONLY);
        if (fd < 0)
        {
            perror("Error opening infile file");
            exit(1);
        }
        dup2(fd, STDIN_FILENO);
    }
    else if (type == 1)
    {
        fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0)
        {
            perror("Error opening output file");
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);
    }
    close(fd);
}

int main(int argc, char **argv)
{
	int fd[2];
    pid_t 	pid1;
	pid_t 	pid2;
    char    **args;

    if (argc != 5)
    {
        printf("Uso: ./pipex infile cmd1 cmd2 outfile\n");
        return 1;
    }
	if(pipe(fd) == -1)
		return 1;
	pid1 = fork();
	if(pid1 < 0)
		return 1;
	if(pid1 == 0)
    {
        infile(argv[1], 0);
        args = get_args(0, argv);
        execute_process(args, fd, 0);
    }
	pid2 = fork();
	if(pid2 < 0)
		return 1;
	if(pid2 == 0)
    {
        infile(argv[4], 1);
        args = get_args(1, argv);
        execute_process(args, fd, 1);
    }
    freeing_memory(fd, pid1, pid2);
	return 0;
}
