/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alistair <alistair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 11:18:31 by alkane            #+#    #+#             */
/*   Updated: 2022/06/02 22:15:58 by alistair         ###   ########.fr       */
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

void	print_message(t_philo *philo, char *msg, int action)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&(data->print_lock));
	if (action)
		printf("%lld %d %s \n", get_time() - data->start_ts, philo->id + 1, "has died");	
	printf("%lld %d %s \n", get_time() - data->start_ts, philo->id + 1, msg);
	pthread_mutex_unlock(&(data->print_lock));
}

void	spend_time(long long time_ms)
{
	long long	end;

	end = get_time() + time_ms;
	while (get_time() < end)
		usleep(time_ms / 1000);
}

void	tidy_up(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->n_philo)
		pthread_mutex_destroy(&(data->fork_array[i]));
	pthread_mutex_destroy(&(data->print_lock));
	pthread_mutex_destroy(&(data->done_lock));
}
