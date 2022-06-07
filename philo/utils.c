/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 11:18:31 by alkane            #+#    #+#             */
/*   Updated: 2022/06/06 22:55:28 by alkane           ###   ########.fr       */
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
	pthread_mutex_lock(&(data->done_lock));
	if (data->done_flag == 0)
		printf("%lld %d %s \n", get_time() - data->start_t, philo->id + 1, msg);
	pthread_mutex_unlock(&(data->done_lock));
}

void	spend_time(long long time_ms)
{
	long long	end;

	end = get_time() + time_ms;
	while (get_time() < end)
		usleep(time_ms / 1000);
}

int	tidy_up(t_data *data)
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
