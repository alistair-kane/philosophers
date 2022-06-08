/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alistair <alistair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 22:51:52 by alistair          #+#    #+#             */
/*   Updated: 2022/06/08 22:53:52 by alistair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	pick_up_forks(t_philo *philo)
{
	sem_wait(philo->data->forks);
	print_message(philo, "has taken a fork [L]");
	sem_wait(philo->data->forks);
	print_message(philo, "has taken a fork [R]");
}

static void	eat(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	sem_wait(philo->checking);
	philo->last_meal = get_time();
	print_message(philo, "is eating");
	philo->n_eaten += 1;	
	if (philo->n_eaten == data->n_meal)
		sem_post(data->finished);
	spend_time(data->tt_eat);
	sem_post(data->forks);
	sem_post(data->forks);
	sem_post(philo->checking);
}

static void	sleeping_thinking(t_philo *philo)
{
	print_message(philo, "is sleeping");
	spend_time(philo->data->tt_sleep);
	print_message(philo, "is thinking");
}

void	ph_func(t_philo *philo)
{
	t_data		*data;
	pthread_t	alive_check;

	data = philo->data;
	pthread_create(&alive_check, NULL, waiter_thread, philo);
	if (philo->id % 2 == 0)
		usleep(data->tt_eat * 1000);
	while (1)
	{
		pick_up_forks(philo);
		eat(philo);
		sleeping_thinking(philo);
	}
	exit (0);
}