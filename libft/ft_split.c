/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yojin <yojin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 22:42:05 by yojin             #+#    #+#             */
/*   Updated: 2023/11/24 14:36:20 by yojin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	get_wordcount(char const *s, char c)
{
	size_t	count;

	if (*s == '\0')
		return (0);
	count = 0;
	if (*s != c)
		count++;
	s++;
	while (*s)
	{
		if (*(s - 1) == c && *s != c)
			count++;
		s++;
	}
	return (count);
}

static void	free_strs(char **strs)
{
	int	i;

	i = 0;
	while (*(strs + i))
		free(*(strs + i++));
	free(strs);
}

static int	split_str(char const *s, char c, char **strs)
{
	size_t	len;
	size_t	count;

	count = 0;
	while (*s)
	{
		if (*s == c)
		{
			s++;
			continue ;
		}
		len = 0;
		while (s[len] && s[len] != c)
			len++;
		strs[count] = ft_substr(s, 0, len);
		if (!strs[count])
		{
			free_strs(strs);
			return (0);
		}
		count++;
		s += len;
	}
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char			**strs;
	size_t			word_count;

	word_count = get_wordcount(s, c);
	strs = (char **)malloc((word_count + 1) * sizeof(char *));
	if (!strs)
		return (0);
	ft_bzero(strs, (word_count + 1) * sizeof(char *));
	if (split_str(s, c, strs) == 0)
		return (0);
	return (strs);
}
