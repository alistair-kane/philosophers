/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 23:26:03 by alistair          #+#    #+#             */
/*   Updated: 2022/05/11 18:57:15 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eating(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&(data->fork_array[philo->left_fork]));
	print_message(philo, "has taken a fork [L]");
	pthread_mutex_lock(&(data->fork_array[philo->right_fork]));
	print_message(philo, "has taken a fork [R]");
	pthread_mutex_lock(&(data->dead_lock));
	philo->last_meal = get_time();
	pthread_mutex_unlock(&(data->dead_lock));
	pthread_mutex_lock(&(data->eat_lock));
	(philo->n_eaten)++;
	pthread_mutex_unlock(&(data->eat_lock));
	print_message(philo, "is eating");
	do_stuff(data, data->tt_eat);
	pthread_mutex_unlock(&(data->fork_array[philo->right_fork]));
	pthread_mutex_unlock(&(data->fork_array[philo->left_fork]));
}

static void	*ph_func(void *arg)
{
	t_data	*data;
	t_philo	*philo;

	philo = (t_philo *)arg;
	data = philo->data;
	if (data->n_philo == 1)
	{
		print_message(philo, "has taken a fork [L]");
		return (NULL);
	}
	if (philo->id % 2 == 0)
		usleep(data->tt_eat * 500);
	while (chk_dead(data) == 0)
	{
		eating(philo);
		if (chk_total_eat(data) == 1)
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

	while (chk_total_eat(d) == 0)
	{
		i = -1;
		while (++i < d->n_philo && (chk_dead(d) == 0))
			chk_any_deaths(p[i]);
		if (chk_dead(d) == 1)
			break ;
		i = 0;
		while (d->n_meal != -1 && i < d->n_philo && \
			chk_ph_meals(p[i]) >= d->n_meal)
			i++;
		if (i == d->n_philo)
		{
			pthread_mutex_lock(&(d->eat_lock));
			d->all_eaten = 1;
			pthread_mutex_unlock(&(d->eat_lock));
		}
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
	pthread_mutex_destroy(&(data->dead_lock));
	pthread_mutex_destroy(&(data->eat_lock));
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
		if (pthread_create(&(philo[i].thread_id), NULL, ph_func, &(philo[i])))
			return (1);
		pthread_mutex_lock(&(data->dead_lock));
		philo[i].last_meal = get_time();
		pthread_mutex_unlock(&(data->dead_lock));
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
