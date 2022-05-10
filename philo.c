/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 23:26:03 by alistair          #+#    #+#             */
/*   Updated: 2022/05/10 18:29:49 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// void	pick_up_fork(t_philo *philo, char c, int *fork)
// {
// 	if (c == 'L')
// 	{
// 		pthread_mutex_lock(philo->left_mutex);
// 		print_message(philo, "has taken a fork [L]");
// 		*fork = 1;
// 		pthread_mutex_unlock(philo->left_mutex);
// 	}
// 	else
// 	{
// 		pthread_mutex_lock(philo->right_mutex);
// 		print_message(philo, "has taken a fork [R]");
// 		*fork = 1;
// 		pthread_mutex_unlock(philo->right_mutex);
// 	}
// }

void	eating(t_philo *philo)
{
	long long	last_meal;

	last_meal = get_time();
	print_message(philo, "is eating");
	// usleep((int)philo->tt_eat * 1000); // could die here while eating
	// if (sleep_or_die(philo, (int)philo->tt_eat, last_meal) == 1)
		// return (1);
	sleep_or_die(philo, (int)philo->tt_eat, last_meal);
	pthread_mutex_lock(philo->left_mutex); 
	*left_fork = pthread_mutex_unlock(philo->left_mutex); // unlock left fork
	pthread_mutex_lock(philo->right_mutex); 
	*right_fork = pthread_mutex_unlock(philo->right_mutex); // unlock right fork
	(*meals)++;
	check_dead(philo, last_meal);
	// if (check_dead(philo, last_meal))
		// return (1);
	if (*meals == philo->min_eat)
	{
		print_message(philo, "IS FULL!!");
		return (1);
	}
	return (0);
}

void	*philosopher(void *arg)
{
	t_data		*data;
	t_philo		*philo;

	philo = (t_philo *)arg;
	data = philo->data;
	if (philo->id % 2 != 0)
		usleep(15000); // delay the start of odd numbered philos
	while (data->dead_flag == 0)
	{
		eating(philo);
		print_message(philo, "is sleeping");
		sleep_or_die(philo, last_meal);
			last_meal = get_time();
		}
		print_message(philo, "is thinking");
		// if (check_dead(philo, last_meal))
		// 	break ;
	}
	return (NULL);
}

void	alive_loop(t_data *d)
{
	t_philo	*p;
	int		i;

	p = d->philos;
	while (d->all_eaten == 0)
	{
		i = -1;
		while (++i < d->n_philo && d->dead_flag == 0)
		{
			pthread_mutex_lock(&(d->meal_lock));
			check_dead(d, p[i]);
			pthread_mutex_unlock(&(d->meal_lock));
		}
		if (d->dead_flag == 1)
			break ;
		// check if _all_ have reached the max meals to be eaten
		i = 0;
		while (d->n_meal != -1 && i < d->n_philo && p[i].n_eaten >= d->n_meal)
			i++;
		if (i == d->n_philo)
			d->all_eaten = 1;
	}
}

// main process where death can be checked
int	start_dinner(t_data *data)
{
	t_philo	*philo;
	int		i;
	
	philo = data->philos;
	data->start_ts = get_time();
	i = -1;
	while (++i < data->n_philo)
	{
		if (pthread_create(&(philo[i].thread_id), NULL, philosopher, &(philo[i])))
			return (1);
		philo[i].last_meal = get_time();
	}
	alive_loop(data);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_data	data;

	if (set_table(&data, argc, argv) == 1)
		return(2);
	if (start_dinner(&data) == 1)
		return (3);
	clear_table(&data);
	return (0);
}