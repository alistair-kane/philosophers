/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alistair <alistair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 11:14:51 by alkane            #+#    #+#             */
/*   Updated: 2022/05/11 23:50:13 by alistair         ###   ########.fr       */
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

static void	philo_init(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->n_philo)
	{
		data->philos[i].id = i;
		data->philos[i].n_eaten = 0;
		data->philos[i].left_fork = i; //int
		data->philos[i].right_fork = (i + 1) % data->n_philo; //int
		data->philos[i].last_meal = 0;
		data->philos[i].data = data;
	}
}

// static int	init_mutexs(t_data *data)
// {
// 	int	i;

// 	i = -1;
// 	while (++i < data->n_philo)
// 		if (pthread_mutex_init(&(data->fork_array[i]), NULL) == 1)
// 			return (1);
// 	if (pthread_mutex_init(&(data->print_lock), NULL) == 1)
// 		return (1);
// 	if (pthread_mutex_init(&(data->dead_lock), NULL) == 1)
// 		return (1);
// 	if (pthread_mutex_init(&(data->eat_lock), NULL) == 1)
// 		return (1);
// 	return (0);
// }

int	set_table(t_data *data, int argc, char **argv)
{
	if (check_input(argc, argv) == 1)
		return (1);
	data->n_philo = ft_atoi(argv[1]);
	data->tt_die = ft_atoi(argv[2]);
	data->tt_eat = ft_atoi(argv[3]);
	data->tt_sleep = ft_atoi(argv[4]);
	

	data->all_eaten = 0; // needs to be semaphore?
	data->dead_flag = 0; // needs to be semaphore?
	if (argv[5] != NULL)
		data->n_meal = ft_atoi(argv[5]);
	else
		data->n_meal = -1;
	// if (init_mutexs(data) == 1)
		// return (1);
	philo_init(data);
	return (0);
}


/*
	constant semaphore for each fork
		wait to "take 'a Left' fork"
		wait to "take 'a Right' fork"
			make a thread to check this?
			eat for the time
		post "the forks back"
		
		if dead post in main thread of all forks?


/*
In the Bonus. 
Every Philosopher has its own thread to check the lifetime.
Because the Philosopher process is waiting for the fork semaphores.  
He can't check its lifetime while waiting. 
So it is outsourced.
*/