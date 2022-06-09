/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 23:26:03 by alistair          #+#    #+#             */
/*   Updated: 2022/06/09 12:30:44 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	pick_up_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	print_message(philo, "has taken a fork [L]");
	pthread_mutex_lock(philo->left_fork);
	print_message(philo, "has taken a fork [R]");
}

static void	eat(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&(philo->checking));
	philo->last_meal = get_time();
	pthread_mutex_lock(&(data->done_lock));
	if (data->done_flag == 0)
		printf("%lld\t%d\t%s\n", get_time() - data->start_t, \
			philo->id + 1, "is eating");
	philo->n_eaten += 1;
	if (philo->n_eaten == data->n_meal)
		data->philos_done_eating += 1;
	pthread_mutex_unlock(&(data->done_lock));
	spend_time(data->tt_eat);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(&(philo->checking));
}

static void	sleeping_thinking(t_philo *philo)
{
	print_message(philo, "is sleeping");
	spend_time(philo->data->tt_sleep);
	print_message(philo, "is thinking");
}

void	*ph_func(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = arg;
	data = philo->data;
	if (philo->id % 2 == 0)
		usleep(data->tt_eat * 1000);
	while (check_done_flag(data) == 0)
	{
		pick_up_forks(philo);
		eat(philo);
		sleeping_thinking(philo);
	}
	return (NULL);
}
