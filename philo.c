/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 23:26:03 by alistair          #+#    #+#             */
/*   Updated: 2022/05/08 21:37:02 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	pick_up_fork(t_philo *philo, char c)
{
	if (c == 'L')
	{
		pthread_mutex_lock(philo->left_mutex);
		print_message(philo, "has taken a fork [L]");
		philo->left_fork = 1;
		pthread_mutex_unlock(philo->left_mutex);
	}
	else
	{
		pthread_mutex_lock(philo->right_mutex);
		print_message(philo, "has taken a fork [R]");
		philo->right_fork = 1;
		pthread_mutex_unlock(philo->right_mutex);
	}
}

int	eat_sleeping(t_philo *philo, uint8_t *meals)
{
	long long	last_meal;

	last_meal = get_time();
	print_message(philo, "is eating");
	// usleep((int)philo->tt_eat * 1000); // could die here while eating
	if (sleep_or_die(philo, (int)philo->tt_eat, last_meal) == 1)
		return (1);
	pthread_mutex_lock(philo->left_mutex); 
	philo->left_fork = pthread_mutex_unlock(philo->left_mutex); // unlock left fork
	pthread_mutex_lock(philo->right_mutex); 
	philo->right_fork = pthread_mutex_unlock(philo->right_mutex); // unlock right fork
	(*meals)++;
	if (check_dead(philo, last_meal))
		return (1);
	if (*meals == philo->min_eat)
	{
		print_message(philo, "IS FULL!!");
		return (1);
	}
	print_message(philo, "is sleeping");
	// usleep((int)philo->tt_sleep * 1000);
	if (sleep_or_die(philo, (int)philo->tt_sleep, last_meal) == 1)
		return (1);
	return (0);
}

void	*philosopher(void *arg)
{
	t_philo		*philo;
	uint8_t		meals;
	long long	last_meal;

	philo = (t_philo *)arg;
	meals = 0;
	last_meal = get_time();
	while (1)
	{
		if (!philo->left_fork)
			pick_up_fork(philo, 'L');
		if (philo->left_fork && philo->right_mutex != NULL && !philo->right_fork)
			pick_up_fork(philo, 'R');
		if (philo->left_fork && philo->right_fork)
		{
			if (eat_sleeping(philo, &meals))
				break ;
			last_meal = get_time();
		}
		print_message(philo, "is thinking");
		if (check_dead(philo, last_meal))
			break ;
	}
	return (NULL);
}

int	main(int argc, char *argv[])
{
	t_data			*data;
	
	data = NULL;
	if (set_table(data, argc, argv) == 1)
		return(2);
	clear_table(data);

	return (0);
}