/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yojin <yojin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 17:26:23 by yojin             #+#    #+#             */
/*   Updated: 2023/11/11 15:17:06 by yojin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	len;
	unsigned int	idx;
	char			*str;

	if (!s)
		return (0);
	len = ft_strlen(s);
	idx = 0;
	str = (char *)malloc((len + 1) * sizeof(char));
	if (!str)
		return (0);
	while (idx < len)
	{
		*(str + idx) = f(idx, *(s + idx));
		idx++;
	}
	*(str + idx) = '\0';
	return (str);
}
