/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alistair <alistair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 11:18:31 by alkane            #+#    #+#             */
/*   Updated: 2022/05/11 22:24:05 by alistair         ###   ########.fr       */
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

void	spend_time(t_data *data, long long stuff_time)
{
	long long	start;

	start = get_time();
	while (chk_dead(data) == 0)
	{
		if (get_time() - start >= stuff_time)
			break ;
		usleep(50);
	}
}

void	tidy_up(t_data *data, t_philo *philos)
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
