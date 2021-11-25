/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_like.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 16:38:59 by mberne            #+#    #+#             */
/*   Updated: 2021/11/25 15:41:28 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && (s1[i] == s2[i]))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	str_isnumber(char *s)
{
	while (*s)
	{
		if (!('0' <= *s && *s <= '9'))
			return (0);
		s++;
	}
	return (1);
}

static int	ft_intlen(long int n)
{
	long int	i;

	i = 0;
	if (n <= 0)
	{
		n *= (-1);
		i++;
	}
	while (n > 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char		*result;
	long int	i;
	int			j;

	i = n;
	j = ft_intlen(n) - 1;
	result = malloc(sizeof(char) * ft_intlen(i) + 1);
	if (!result)
		return (NULL);
	if (i < 0)
	{
		result[0] = '-';
		i = -i;
	}
	else if (i == 0)
		result[0] = '0';
	while (i > 0)
	{
		result[j--] = i % 10 + '0';
		i /= 10;
	}
	result[ft_intlen(n)] = '\0';
	return (result);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	n;
	int	nb;
	int	len;

	i = 0;
	n = 1;
	nb = 0;
	len = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-')
		n *= (-1);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] >= 48 && str[i] <= 57)
	{
		nb = nb * 10 + (str[i++] - '0');
		len++;
	}
	return (nb * n);
}
