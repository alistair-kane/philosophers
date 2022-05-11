/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alistair <alistair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 11:18:31 by alkane            #+#    #+#             */
/*   Updated: 2022/05/11 02:37:12 by alistair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(void)
{
	struct timeval	tv;
	long long		milliseconds;

	gettimeofday(&tv, NULL);
	milliseconds = tv.tv_sec * 1000LL + tv.tv_usec / 1000;
	return (milliseconds);
}

void	print_message(t_philo *philo, char *msg)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&(data->print_lock));
	if (data->dead_flag == 0)
		printf("%lld %d %s \n", get_time() - data->start_ts, philo->id + 1, msg);
	pthread_mutex_unlock(&(data->print_lock));
}

void	check_dead(t_philo philo)
{
	t_data	*data;

	data = philo.data;
	pthread_mutex_lock(&(data->meal_lock));
	if ((get_time() - philo.last_meal) > data->tt_die)
	{
		print_message(&philo, "died");
		data->dead_flag = 1;
	}
	pthread_mutex_unlock(&(data->meal_lock));
	usleep(100); // why sleep here?
}

void	do_stuff(t_data *data, long long stuff_time)
{
	long long	start;

	start = get_time();
	while (data->dead_flag == 0)
	{
		if (get_time() - start >= stuff_time)
			break ;
		usleep(10);
	}
}

void	eating(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&(data->fork_array[philo->left_fork]));
	print_message(philo, "has taken a fork [L]");
	pthread_mutex_lock(&(data->fork_array[philo->right_fork]));
	print_message(philo, "has taken a fork [R]");
	pthread_mutex_lock(&(data->meal_lock));
	print_message(philo, "is eating");
	philo->last_meal = get_time();
	pthread_mutex_unlock(&(data->meal_lock));
	do_stuff(data, data->tt_eat);
	(philo->n_eaten)++;
	pthread_mutex_unlock(&(data->fork_array[philo->left_fork]));
	pthread_mutex_unlock(&(data->fork_array[philo->right_fork]));
}
