#include "pipex.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	total_len;
	int		i;
	size_t	fst_len;
	char	*new_str;

	if (!s1 || !s2)
		return (NULL);
	fst_len = strlen(s1);
	total_len = fst_len + strlen(s2) + 1;
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
