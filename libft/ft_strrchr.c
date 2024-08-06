/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yojin <yojin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 17:04:04 by yojin             #+#    #+#             */
/*   Updated: 2023/10/28 20:21:55 by yojin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*ptr;
	size_t	i;

	i = ft_strlen(s);
	ptr = (char *)s;
	while (i > 0 && *(ptr + i) != (char)c)
		i--;
	if (*(ptr + i) == (char)c)
		return (ptr + i);
	return (0);
}
