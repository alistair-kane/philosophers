/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 12:24:54 by alkane            #+#    #+#             */
/*   Updated: 2022/06/09 12:34:43 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	start_dinner(t_data *data)
{
	pthread_t	monitor;
	int			i;

	data->start_t = get_time();
	i = 0;
	while (i < data->n_philo)
	{
		data->philos[i].last_meal = data->start_t;
		if (pthread_create(&data->philos[i].thread_id, NULL, \
			ph_func, &data->philos[i]))
			return (1);
		pthread_create(&monitor, NULL, waiter_thread, &data->philos[i]);
		pthread_detach(monitor);
		++i;
	}
	if (data->n_meal > 0)
	{
		pthread_create(&monitor, NULL, finish_eating_waiter, data);
		pthread_detach(monitor);
	}
	return (0);
}

static int	tidy_up(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philo)
	{
		pthread_join(data->philos[i].thread_id, NULL);
		pthread_mutex_destroy(&(data->philos[i++].checking));
	}
	i = 0;
	while (i < data->n_philo)
		pthread_mutex_destroy(&(data->fork_array[i++]));
	return (0);
}

int	main(int argc, char *argv[])
{
	t_data	data;

	if (set_table(&data, argc, argv) == 1)
		return (2);
	if (start_dinner(&data) == 1)
		return (3);
	if (tidy_up(&data) == 1)
		return (4);
	return (0);
}
