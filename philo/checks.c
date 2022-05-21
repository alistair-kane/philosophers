/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 19:04:09 by alkane            #+#    #+#             */
/*   Updated: 2022/05/16 13:33:28 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	chk_philo_death(t_philo philo)
{
	t_data	*data;

	data = philo.data;
	pthread_mutex_lock(&(data->dead_lock));
	if ((get_time() - philo.last_meal) > data->tt_die)
	{
		print_message(&philo, "died");
		data->dead_flag = 1;
	}
	pthread_mutex_unlock(&(data->dead_lock));
}

int	chk_dead(t_data *data)
{
	int	temp;

	pthread_mutex_lock(&(data->dead_lock));
	temp = data->dead_flag;
	pthread_mutex_unlock(&(data->dead_lock));
	return (temp);
}

int	chk_ph_meals(t_philo philo)
{
	t_data	*data;
	int		temp;

	data = philo.data;
	pthread_mutex_lock(&(data->eat_lock));
	temp = philo.n_eaten;
	pthread_mutex_unlock(&(data->eat_lock));
	return (temp);
}

int	chk_total_eat(t_data *data)
{
	int	temp;

	pthread_mutex_lock(&(data->eat_lock));
	temp = data->all_eaten;
	pthread_mutex_unlock(&(data->eat_lock));
	return (temp);
}
