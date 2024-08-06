/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yojin <yojin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 15:50:50 by yojin             #+#    #+#             */
/*   Updated: 2023/10/28 16:05:27 by yojin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*_dest;
	unsigned char	*_src;
	size_t			i;

	if (dest == src || n == 0)
		return (dest);
	i = 0;
	_dest = dest;
	_src = (unsigned char *)src;
	while (i < n)
	{
		_dest[i] = _src[i];
		i++;
	}
	return (dest);
}
