/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alistair <alistair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 23:26:03 by alistair          #+#    #+#             */
/*   Updated: 2022/05/11 02:50:13 by alistair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*philosopher(void *arg)
{
	t_data	*data;
	t_philo	*philo;

	philo = (t_philo *)arg;
	data = philo->data;
	if (philo->id % 2 != 0)
		usleep(15000); // delay the start of odd numbered philos
	while (data->dead_flag == 0)
	{
		eating(philo);
		if (data->all_eaten == 1)
			break ;
		print_message(philo, "is sleeping");
		do_stuff(data, data->tt_sleep);
		print_message(philo, "is thinking");
	}
	return (NULL);
}

static void	alive_loop(t_data *d, t_philo *p)
{
	int	i;

	while (d->all_eaten == 0)
	{
		i = -1;
		while (++i < d->n_philo && d->dead_flag == 0)
			check_dead(p[i]);
		if (d->dead_flag == 1)
			break ;
		// check if _all_ have reached the max meals to be eaten
		i = 0;
		while (d->n_meal != -1 && i < d->n_philo && p[i].n_eaten >= d->n_meal)
			i++;
		if (i == d->n_philo)
			d->all_eaten = 1;
	}
}

static void	tidy_up(t_data *data, t_philo *philos)
{
	int	i;

	i = -1;
	while (++i < data->n_philo)
		pthread_join(philos[i].thread_id, NULL);
	i = -1;
	while (++i < data->n_philo)
		pthread_mutex_destroy(&(data->fork_array[i]));
	pthread_mutex_destroy(&(data->print_lock));
	pthread_mutex_destroy(&(data->meal_lock));
}

// main process where death can be checked
static int	start_dinner(t_data *data)
{
	t_philo	*philo;
	int		i;

	philo = data->philos;
	data->start_ts = get_time();
	i = -1;
	while (++i < data->n_philo)
	{
		if (pthread_create(&(philo[i].thread_id), NULL, \
			philosopher, &(philo[i])))
			return (1);
		philo[i].last_meal = get_time();
	}
	alive_loop(data, data->philos);
	tidy_up(data, philo);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_data	data;

	if (set_table(&data, argc, argv) == 1)
		return (2);
	if (start_dinner(&data) == 1)
		return (3);
	return (0);
}
