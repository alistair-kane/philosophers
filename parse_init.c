/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 11:14:51 by alkane            #+#    #+#             */
/*   Updated: 2022/05/10 18:13:05 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	convert(long temp_l)
{
	if (temp_l > INT32_MAX || temp_l < 1)
		return (0);
	return((int)temp_l);
}

static int	check_input(int	argc, char **argv)
{
	int	tt_die;
	int	tt_eat;
	int	tt_sleep;

	if (argc >= 5 && argc <= 6)
	{
		if (ft_atoi(argv[1]) > 255 || ft_atoi(argv[1]) < 1)
			return (1);
		tt_die = convert(ft_atoi(argv[2]));
		tt_eat = convert(ft_atoi(argv[3]));
		tt_sleep = convert(ft_atoi(argv[4]));
		if (tt_die == 0 || tt_eat == 0 || tt_sleep == 0)
			return (1);
		// need to check if optional arg is passed
		if (argv[5] != NULL)
		{
			if (convert(ft_atoi(argv[5])) == 0)
				return (1);
		}
	}
	else
		return (1);
	return (0);
}

void	philo_init(t_data *data)
{
	int	i;

	i = -1; // init the threads for the philosopher function
	while (++i < data->n_philo)
	{
		data->philos[i].id = i;
		data->philos[i].n_eaten = 0;
		data->philos[i].left_fork = i;
		data->philos[i].right_fork = (i + 1) % data->n_philo;
		data->philos[i].last_meal = 0;
		data->philos[i].data = data;
	}
}

int	set_table(t_data *data, int argc, char **argv)
{
	int		i;
	
	if (check_input(argc, argv) == 1)
		return (1);
	data->n_philo = ft_atoi(argv[1]);
	data->tt_die = ft_atoi(argv[2]);
	data->tt_eat = ft_atoi(argv[3]);
	data->tt_sleep = ft_atoi(argv[4]);
	data->all_eaten = 0;
	data->dead_flag = 0;
	if (argv[5] != NULL)
		data->n_meals = ft_atoi(argv[5]);
	else
		data->n_meals = -1;

	i = -1;
	// init mutexes
	while (++i < data->n_philo)
		pthread_mutex_init(&(data->fork_array[i]), NULL); // check error?
	pthread_mutex_init(&(data->print_lock), NULL); // check error?
	pthread_mutex_init(&(data->meal_lock), NULL); // check error?
	philo_init(data);
	// i = -1;
	// while (++i < data->n)
	// 	pthread_join(data->thread_array[i], NULL);
	return (0);
}