#include "pipex.h"

static char *get_path_from_env(char **envp)
{
    int i = 0;
    
    while (envp[i])
    {
        if (envp[i][0] == 'P' && envp[i][1] == 'A' && 
            envp[i][2] == 'T' && envp[i][3] == 'H' && envp[i][4] == '=')
            return (envp[i] + 5);
        i++;
    }
    return (NULL);
}

static void freeing_memory(int *fd, pid_t pid1, pid_t pid2)
{
    int status;

	close(fd[0]);
    close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);

    if (status >= 0)
        exit((status >> 8) & 0xFF);
    else
        exit(1);
}

static void execute_process(char** args, int *pipefd, int type, char **envp)
{
	char	**env_path;
	char	*bin_path;
    char    *env;
    int         i;

    env = get_path_from_env(envp);
	env_path = ft_split(env, ':');
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
	if(execve(bin_path, args, envp) == -1)
    {
        write(2, "command not found\n", 18);
        exit(127);
    }
}

int main(int argc, char **argv, char **envp)
{
	int fd[2];
    pid_t 	pid1;
	pid_t 	pid2;
    char    **args;
    int file;

    if (argc != 5)
    {
        write(2, "Invalid arguments, please try again:\n./pipex infile cmd1 cmd2 outfile\n", 70);
        return 1;
    }
	if(pipe(fd) == -1)
		return 1;
	pid1 = fork();
	if(pid1 < 0)
		return 1;
	if(pid1 == 0)
    {
        file = open(argv[1], O_RDONLY);
        if (file < 0)
        {
            write(2, "Error opening input file\n", 25);
            exit(1);
        }
        dup2(file, STDIN_FILENO);
        dup2(fd[1], STDOUT_FILENO);
        args = get_args(0, argv);
        execute_process(args, fd, 0, envp);
    }
	pid2 = fork();
	if(pid2 < 0)
		return 1;
	if(pid2 == 0)
    {
        file = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (file < 0)
        {
            write(2, "Error opening output file\n", 26);
            exit(1);
        }
        dup2(fd[0], STDIN_FILENO);
        dup2(file, STDOUT_FILENO);
        args = get_args(1, argv);
        execute_process(args, fd, 1, envp);
    }
    freeing_memory(fd, pid1, pid2);
	return 0;
}
