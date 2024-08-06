/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yojin <yojin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 17:02:33 by yojin             #+#    #+#             */
/*   Updated: 2023/10/28 21:01:38 by yojin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*ptr;
	size_t			i;

	ptr = (unsigned char *)s;
	i = 0;
	if (n == 0)
		return (0);
	while (i + 1 < n && *(ptr + i) != (unsigned char)c)
		i++;
	if (*(ptr + i) == (unsigned char)c)
		return (ptr + i);
	return (0);
}
