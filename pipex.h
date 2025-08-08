#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>     // fork, pipe, dup2, execve, write, close, etc.
# include <stdlib.h>     // malloc, free, exit
# include <stdio.h>      // perror, printf
# include <fcntl.h>      // open y flags como O_RDONLY, O_WRONLY
# include <sys/types.h>  // pid_t
# include <sys/wait.h>   // wait, waitpid
# include <string.h>     // strerror, strlen, strlcpy
# include <errno.h>      // acceder a errno y mostrar errores

int     count_words(char *str, char delimiter);
char	*ft_strjoin(char const *s1, char const *s2);
char    *command_args(char *str);
char    *command_clear(char *str);
char	**ft_split(const char *s, char c);

#endif
