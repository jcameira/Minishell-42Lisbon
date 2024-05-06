# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <stdbool.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <dirent.h>
# include <termios.h>
// # include <libft.h>

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strnstr(const char *str, const char *substr, size_t len)
{
	size_t	i;
	size_t	j;

	if (!str && !substr)
		return (NULL);
	if (substr == str || ft_strlen(substr) == 0)
		return ((char *)str);
	i = 0;
	while (i < len && str[i])
	{
		j = 0;
		while (str[i + j] && substr[j] && str[i + j] == substr[j]
			&& (i + j) < len)
			j++;
		if (!substr[j])
			return ((char *)(str + i));
		i++;
	}
	return (NULL);
}

int	ft_strncmp(const char *str1, const char *str2, size_t n)
{
	size_t	i;

	if (n == 0)
		return (0);
	i = 0;
	while (i < n - 1 && str1[i] && str1[i] == str2[i])
		i++;
	return ((unsigned char)str1[i] - (unsigned char)str2[i]);
}

int	main(int ac, char **av)
{
	char	tmp[_PC_PATH_MAX];
	// int		i;
	// int 	j;

	// i = -1;
	ac = 0;
	av = NULL;

	if (getcwd(tmp, sizeof(char)) && *tmp)
		printf("%s\n", tmp);
}