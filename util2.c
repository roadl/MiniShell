#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	if (!s1 || !s2)
		return (1);
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

t_cmd	*index_cmd(t_list *lst, int index)
{
	t_list	*node;
	int		i;

	node = lst;
	i = 0;
	while (node)
	{
		if (i == index)
			return (node->content);
		node = node->next;
		i++;
	}
	return (NULL);
}

char	*ft_strjoin_with_free(char *s1, char *s2)
{
	char	*join;
	size_t	i;

	join = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!join)
		handle_systemcall_error();
	i = 0;
	while (i < ft_strlen(s1))
	{
		join[i] = s1[i];
		i++;
	}
	while (i < ft_strlen(s1) + ft_strlen(s2))
	{
		join[i] = s2[i - ft_strlen(s1)];
		i++;
	}
	join[i] = '\0';
	free(s1);
	free(s2);
	return (join);
}

char	*ft_strncpy(char *dest, char *src, unsigned int n)
{
	unsigned int	i;
	unsigned int	size;

	size = ft_strlen(src);
	i = 0;
	while (i < n)
	{
		if (i >= size)
		{
			dest[i] = '\0';
		}
		else
		{
			dest[i] = src[i];
		}
		i++;
	}
	return (dest);
}

size_t	ft_strslen(char **strs)
{
	size_t  i;

	i = 0;
	while (strs[i])
		i++;
	return (i);
}
