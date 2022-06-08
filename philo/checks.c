/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alistair <alistair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 19:04:09 by alkane            #+#    #+#             */
/*   Updated: 2022/06/08 00:19:04 by alistair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_done_flag(t_data *data)
{
	int	temp;

	pthread_mutex_lock(&(data->done_lock));
	temp = data->done_flag;
	pthread_mutex_unlock(&(data->done_lock));
	return (temp);
}

void	*monitor_thread(void *arg)
{
	t_philo		*philo;
	t_data		*data;
	long long	time;

	philo = arg;
	data = philo->data;
	while (check_done_flag(data) == 0)
	{
		pthread_mutex_lock(&(philo->checking));
		pthread_mutex_lock(&(data->done_lock));
		time = get_time() - philo->last_meal;
		if (time >= data->tt_die && data->done_flag == 0)
		{
			printf("%lld %d %s \n", get_time() - data->start_t,
				philo->id + 1, "died");
			data->done_flag = 1;
		}
		pthread_mutex_unlock(&(data->done_lock));
		pthread_mutex_unlock(&(philo->checking));
	}
	return (NULL);
}

void	*monitor_thread_eat(void *arg)
{
	t_data	*data;

	data = arg;
	while (data->done_flag == 0)
	{
		pthread_mutex_lock(&(data->done_lock));
		if (data->philos_done_eating == data->n_philo)
			data->done_flag = 1;
		pthread_mutex_unlock(&(data->done_lock));
	}
	return (NULL);
}
