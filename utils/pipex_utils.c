#include "../pipex.h"

static size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
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

char *find_binary(char *command, char **paths)
{
    char	*full_path;
	int		i;
    char    *temp;

	i = 0;
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

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	src_len;
	size_t	i;

	src_len = ft_strlen(src);
	i = 0;
	if (size == 0)
		return (src_len);
	while (i < size - 1 && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (src_len);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	total_len;
	int		i;
	size_t	fst_len;
	char	*new_str;

	if (!s1 || !s2)
		return (NULL);
	fst_len = ft_strlen(s1);
	total_len = fst_len + ft_strlen(s2) + 1;
	new_str = malloc(total_len);
	if (!new_str)
		return (NULL);
	i = -1;
	while (s1[++i])
		new_str[i] = s1[i];
	i = 0;
	while (s2[i])
	{
		new_str[fst_len + i] = s2[i];
		i++;
	}
	new_str[fst_len + i] = '\0';
	return (new_str);
}
