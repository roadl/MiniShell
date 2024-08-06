/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yojin <yojin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 17:02:44 by yojin             #+#    #+#             */
/*   Updated: 2023/11/24 14:17:03 by yojin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	get_digit(long long n)
{
	int	digit;

	digit = 1;
	if (n < 0)
	{
		digit++;
		n *= -1;
	}
	while (n >= 10)
	{
		n /= 10;
		digit++;
	}
	return (digit);
}

char	*ft_itoa(int n)
{
	char		*str;
	int			digit;
	long long	num;

	num = n;
	digit = get_digit(num);
	str = (char *)malloc((digit + 1) * sizeof(char));
	if (!str)
		return (0);
	*(str + digit) = '\0';
	digit--;
	if (num < 0)
	{
		*str = '-';
		num *= -1;
	}
	while (num >= 10)
	{
		*(str + digit) = num % 10 + '0';
		num /= 10;
		digit--;
	}
	*(str + digit) = num % 10 + '0';
	return (str);
}
