/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alistair <alistair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 23:26:03 by alistair          #+#    #+#             */
/*   Updated: 2022/06/03 13:44:32 by alistair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	pick_up_forks(t_philo *philo)
{
	t_data	*data;
	int		fork;
	int		i;

	data = philo->data;
	i = 0;
	while (i < 2)
	{
		if (chk_philo_death(philo) == 1)
			return (1);
		if (i == 0)
			fork = philo->left_fork;
		else
			fork = philo->right_fork;
		pthread_mutex_lock(&(data->fork_array[fork]));
		if (data->fork_states[fork] == 0)
		{
			if (i == 0)
				print_message(philo, "has taken a fork [L]", 0);
			else
				print_message(philo, "has taken a fork [R]", 0);
			data->fork_states[fork] = 1;
			i++;
		}
		pthread_mutex_unlock(&(data->fork_array[fork]));
	}
	return (0);
}

int	eating(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	philo->eating = 1;
	philo->last_meal = get_time();
	print_message(philo, "is eating", 0);
	spend_time(data->tt_eat);
	philo->limit = philo->last_meal + data->tt_die;
	philo->eating = 0;
	philo->n_eaten += 1;
	if (data->n_meal > 0 && philo->n_eaten >= data->n_meal)
	{
		pthread_mutex_lock(&(data->fork_array[philo->left_fork]));
		data->fork_states[philo->left_fork] = 0;
		pthread_mutex_unlock(&(data->fork_array[philo->left_fork]));
		pthread_mutex_lock(&(data->fork_array[philo->right_fork]));
		data->fork_states[philo->right_fork] = 0;
		pthread_mutex_unlock(&(data->fork_array[philo->right_fork]));
		pthread_mutex_lock(&(data->meal_lock));
		data->done_flag++;
		if (data->done_flag >= data->n_philo)
		{
			pthread_mutex_unlock(&(data->meal_lock));
			pthread_mutex_lock(&(data->print_lock));
			// pthread_mutex_lock(&(data->done_lock));
			pthread_mutex_unlock(&(data->done_lock));
		}
		else
			pthread_mutex_unlock(&(data->meal_lock));
		// else
			// pthread_mutex_unlock(&(data->done_lock));
		return (1);
	}
	return (0);
}

int	put_down_forks(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&(data->fork_array[philo->left_fork]));
	data->fork_states[philo->left_fork] = 0;
	pthread_mutex_unlock(&(data->fork_array[philo->left_fork]));
	pthread_mutex_lock(&(data->fork_array[philo->right_fork]));
	data->fork_states[philo->right_fork] = 0;
	pthread_mutex_unlock(&(data->fork_array[philo->right_fork]));
	print_message(philo, "is sleeping", 0);
	spend_time(data->tt_sleep);
	if (chk_philo_death(philo) == 1)
		return (1);
	print_message(philo, "is thinking", 0);
	return (0);
}

static void	*ph_func(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	philo->last_meal = get_time();
	philo->limit = philo->last_meal + data->tt_die;
	while (1)
	{
		if (pick_up_forks(philo) || eating(philo) || put_down_forks(philo))
		{
			break ;
		}
	}
	return (NULL);
}

static int	start_dinner(t_data *data)
{
	pthread_t	thread_id;
	int		i;

	i = -1;
	pthread_mutex_lock(&(data->done_lock));
	data->start_ts = get_time();
	while (++i < data->n_philo)
	{
		if (pthread_create(&thread_id, NULL, &ph_func, (void *)&data->philos[i]))
			return (1);
		pthread_detach(thread_id);
		if (data->n_philo % 2)
			usleep(500);
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_data	data;

	if (set_table(&data, argc, argv) == 1)
		return (2);
	if (start_dinner(&data) == 1)
		return (3);
	pthread_mutex_lock(&(data.done_lock));
	pthread_mutex_unlock(&(data.done_lock));
	// tidy_up(&data);
	return (0);
}
