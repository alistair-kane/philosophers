/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waiters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 19:04:09 by alkane            #+#    #+#             */
/*   Updated: 2022/06/09 12:39:45 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*waiter_thread(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = arg;
	data = philo->data;
	while (1)
	{
		sem_wait(philo->checking);
		sem_wait(data->print_lock);
		if ((get_time() - philo->last_meal) > data->tt_die)
		{
			printf("%lld\t%d\t%s\n", get_time() - data->start_ts, \
				philo->id + 1, "died");
			sem_post(data->done_lock);
			return (NULL);
		}
		sem_post(data->print_lock);
		sem_post(philo->checking);
	}
	return (NULL);
}

void	*finish_eating_waiter(void *arg)
{
	t_data	*data;
	int		i;

	data = arg;
	i = -1;
	while (++i < data->n_philo)
		sem_wait(data->finished);
	sem_post(data->done_lock);
	return (NULL);
}

void	*finish_waiter(void *arg)
{
	t_data	*data;
	int		i;

	data = arg;
	sem_wait(data->done_lock);
	i = -1;
	while (++i < data->n_philo)
		kill(data->philos[i].pid, SIGTERM);
	return (NULL);
}
