/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yojin <yojin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 22:24:17 by yojin             #+#    #+#             */
/*   Updated: 2023/11/11 15:06:38 by yojin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_incharset(const char c, char const *set)
{
	size_t	i;

	i = 0;
	while (*(set + i))
	{
		if (*(set + i) == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	front;
	size_t	back;

	if (ft_strlen(s1) == 0)
		return (ft_substr("", 0, 0));
	front = 0;
	back = ft_strlen(s1) - 1;
	while (*(s1 + front) && ft_incharset(*(s1 + front), set))
		front++;
	while (back >= 0 && ft_incharset(*(s1 + back), set))
		back--;
	return (ft_substr(s1, front, back - front + 1));
}
