/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 11:14:51 by alkane            #+#    #+#             */
/*   Updated: 2022/06/09 12:19:16 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	convert(long temp_l)
{
	if (temp_l > INT32_MAX || temp_l < 1)
		return (0);
	return ((int)temp_l);
}

static int	check_input(int argc, char **argv)
{
	int	tt_die;
	int	tt_eat;
	int	tt_sleep;

	if (argc >= 5 && argc <= 6)
	{
		if (ft_atoi(argv[1]) > 250 || ft_atoi(argv[1]) < 1)
			return (1);
		tt_die = convert(ft_atoi(argv[2]));
		tt_eat = convert(ft_atoi(argv[3]));
		tt_sleep = convert(ft_atoi(argv[4]));
		if (tt_die == 0 || tt_eat == 0 || tt_sleep == 0)
			return (1);
		if (argv[5] != NULL)
		{
			if (convert(ft_atoi(argv[5])) == 0)
				return (1);
		}
		return (0);
	}
	return (1);
}

static int	philo_init(t_data *data)
{
	int	i;
	int	n;

	if (pthread_mutex_init(&(data->done_lock), NULL) == 1)
		return (1);
	i = 0;
	n = data->n_philo;
	while (i < n)
	{
		data->philos[i].id = i;
		if (pthread_mutex_init(&(data->fork_array[i]), NULL) == 1)
			return (1);
		if (pthread_mutex_init(&(data->philos[i].checking), NULL) == 1)
			return (1);
		if (i == 0)
			data->philos[i].left_fork = &(data->fork_array[n - 1]);
		else
			data->philos[i].left_fork = &(data->fork_array[i - 1]);
		data->philos[i].right_fork = &(data->fork_array[i]);
		data->philos[i].data = data;
		++i;
	}
	return (0);
}

int	set_table(t_data *data, int argc, char **argv)
{
	if (check_input(argc, argv) == 1)
		return (1);
	data->n_philo = ft_atoi(argv[1]);
	data->tt_die = ft_atoi(argv[2]);
	data->tt_eat = ft_atoi(argv[3]);
	data->tt_sleep = ft_atoi(argv[4]);
	if (argv[5] != NULL)
		data->n_meal = ft_atoi(argv[5]);
	else
		data->n_meal = -1;
	data->done_flag = 0;
	if (philo_init(data) == 1)
		return (1);
	return (0);
}
