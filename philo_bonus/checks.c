/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 19:04:09 by alkane            #+#    #+#             */
/*   Updated: 2022/05/21 19:00:27 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	chk_philo_death(t_philo philo)
{
	t_data	*data;

	data = philo.data;
	// printf("philo: [%d] - last meal:[%lld] | current time:[%lld]\n", \
	// 	philo.id, philo.last_meal, get_time());
	if ((get_time() - philo.last_meal) > data->tt_die)
	{
		take_sem(data->dead_lock);
		print_message(&philo, "died");
		data->dead_flag = 1;
		release_sema(data->dead_lock);
		return (1);
	}
	return (0);
}

int	chk_dead_flag(t_data *data)
{
	int		temp;

	take_sem(data->dead_lock);
	temp = data->dead_flag;
	release_sema(data->dead_lock);
	return (temp);
}

// int	chk_ph_meals(t_philo philo)
// {
// 	t_data	*data;
// 	int		temp;

// 	data = philo.data;
// 	pthread_mutex_lock(&(data->eat_lock));
// 	temp = philo.n_eaten;
// 	return (temp);
// }

// int	chk_total_eat(t_data *data)
// {
// 	int	temp;

// 	pthread_mutex_lock(&(data->eat_lock));
// 	temp = data->all_eaten;
// 	return (temp);
// }
