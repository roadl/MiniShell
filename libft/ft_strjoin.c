/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yojin <yojin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 22:13:05 by yojin             #+#    #+#             */
/*   Updated: 2023/10/28 22:18:36 by yojin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	s_len;
	size_t	i;

	s_len = ft_strlen(s1);
	str = malloc((s_len + ft_strlen(s2) + 1) * sizeof(char));
	if (!str)
		return (0);
	i = 0;
	while (*(s1 + i))
	{
		*(str + i) = *(s1 + i);
		i++;
	}
	i = 0;
	while (*(s2 + i))
	{
		*(str + s_len + i) = *(s2 + i);
		i++;
	}
	*(str + s_len + i) = '\0';
	return (str);
}
