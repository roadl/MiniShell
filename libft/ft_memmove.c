/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yojin <yojin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 16:07:08 by yojin             #+#    #+#             */
/*   Updated: 2023/10/28 16:35:17 by yojin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*_dest;
	unsigned char	*_src;
	size_t			i;

	if (dest == src || n == 0)
		return (dest);
	_dest = dest;
	_src = (unsigned char *)src;
	if (_dest < _src + n && _dest > _src)
	{
		i = n + 1;
		while (--i > 0)
			_dest[i - 1] = _src[i - 1];
		return (dest);
	}
	i = -1;
	while (++i < n)
		_dest[i] = _src[i];
	return (dest);
}
