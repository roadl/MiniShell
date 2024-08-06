/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yojin <yojin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 17:26:23 by yojin             #+#    #+#             */
/*   Updated: 2023/10/31 17:39:09 by yojin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char const *s, void (*f)(unsigned int, char *))
{
	unsigned int	len;
	unsigned int	idx;

	len = ft_strlen(s);
	idx = 0;
	while (idx < len)
	{
		f(idx, (char *)(s + idx));
		idx++;
	}
}
