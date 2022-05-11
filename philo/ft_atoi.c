/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alistair <alistair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 21:01:58 by alkane            #+#    #+#             */
/*   Updated: 2022/04/27 02:18:21 by alistair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_space(char ch)
{
	if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\v' || ch == '\f'
		|| ch == '\r')
		return (1);
	else
		return (0);
}

static int	check_negative(const char *nptr, unsigned int *idx)
{
	int negative_count;
	unsigned int	i;
	
	negative_count = 1;
	i = 0;
	while ((is_space(nptr[i]) || nptr[i] == '-' || nptr[i] == '+'))
	{
		if ((nptr[i] == '-') || (nptr[i] == '+'))
		{
			if (nptr[i++] == '-')
				negative_count = -1;
			break ;
		}
		i++;
	}
	*idx = i;
	return(negative_count);
}

long	ft_atoi(const char *nptr)
{
	unsigned int	i;
	int				negative_count;
	long			return_value;

	i = -1;
	// negative_count = 1;
	negative_count = check_negative(nptr, &i);
	return_value = 0;
	// while ((is_space(nptr[++i]) || nptr[i] == '-' || nptr[i] == '+'))
	// {
	// 	if ((nptr[i] == '-') || (nptr[i] == '+'))
	// 	{
	// 		if (nptr[i++] == '-')
	// 			negative_count = -1;
	// 		break ;
	// 	}
	// }
	if (!nptr[i])
		return (2147483648);
	while (nptr[i])
	{
		if (nptr[i] >= '0' && nptr[i] <= '9')
			return_value = return_value * 10 + (nptr[i++] - '0');
		else
			return (2147483648);
	}
	return (return_value * negative_count);
}
