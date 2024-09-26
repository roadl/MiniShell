#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

char	**remove_str_from_array(char **arr, int index)
{
	int i, j;
	int len;
	char **new_arr;

	len = 0;
	for (i = 0; arr[i]; i++)
		len++;
	new_arr = (char **)malloc(sizeof(char *) * (len + 1));
	if (!new_arr)
		return NULL;
	for (i = 0, j = 0; arr[i]; i++)
	{
		if (i != index)
		{
			new_arr[j] = strdup(arr[i]);
			j++;
		}
	}
	new_arr[j] = NULL;
	for (i = 0; arr[i]; i++)
		free(arr[i]);
	free(arr);
	return new_arr;
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