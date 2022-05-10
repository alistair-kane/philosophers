/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 11:56:50 by alkane            #+#    #+#             */
/*   Updated: 2022/05/09 12:47:38 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

uint16_t	convert(long temp_l, int bytes)
{
	if (bytes == 16)
	{
		if (temp_l > UINT16_MAX || temp_l < 1)
			return (0);
		return((uint16_t)temp_l);
	}
	if (bytes == 8)
	{
		if (temp_l > UINT8_MAX || temp_l < 1)
			return (0);
		return((uint8_t)temp_l);
	}
	return (0);
}

int	check_input(char **argv)
{
	uint16_t	tt_die;
	uint16_t	tt_eat;
	uint16_t	tt_sleep;

	if (ft_atoi(argv[1]) > 255 || ft_atoi(argv[1]) < 1)
		return (1);
	tt_die = convert(ft_atoi(argv[2]), 16);
	tt_eat = convert(ft_atoi(argv[3]), 16);
	tt_sleep = convert(ft_atoi(argv[4]), 16);
	if (tt_die == 0 || tt_eat == 0|| tt_sleep == 0)
		return (1);
	// need to check if optional arg is passed
	if (argv[5] != NULL)
	{
		if (convert(ft_atoi(argv[5]), 8) == 0)
			return (1);
	}
	return (0);
}

t_data	data_init(t_data data, char **argv, pthread_mutex_t *print_lock)
{
	struct timeval	tv;
	long long		milliseconds;
	
	gettimeofday(&tv, NULL);
	milliseconds = tv.tv_sec * 1000LL + tv.tv_usec / 1000;
	data.tt_die = convert(ft_atoi(argv[2]), 16);
	data.tt_eat = convert(ft_atoi(argv[3]), 16);
	data.tt_sleep = convert(ft_atoi(argv[4]), 16);
	// need to check if optional arg is passed
	if (argv[5] != NULL)
		data.min_eat = convert(ft_atoi(argv[5]), 8);
	else
		data.min_eat = UINT8_MAX;
	data.left_fork = 0;
	data.right_fork = 0;
	data.start_time = milliseconds;
	data.print_lock = print_lock;
	
	return (data);
}