/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yojin <yojin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 17:07:57 by yojin             #+#    #+#             */
/*   Updated: 2023/11/11 15:24:15 by yojin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;
	char	*ptr;
	size_t	needle_len;

	ptr = (char *)haystack;
	needle_len = ft_strlen(needle);
	if (needle_len == 0)
		return ((char *)haystack);
	i = 0;
	while (*ptr && i + needle_len - 1 < len)
	{
		j = 0;
		while (*(ptr + j) && *(needle + j) && i + j < len)
		{
			if (*(ptr + j) != *(needle + j))
				break ;
			j++;
		}
		if (*(needle + j) == '\0')
			return (ptr);
		ptr++;
		i++;
	}
	return (0);
}
