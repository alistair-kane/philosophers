/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 19:04:09 by alkane            #+#    #+#             */
/*   Updated: 2022/05/14 12:01:07 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	chk_philo_death(t_philo philo)
{
	t_data	*data;

	data = philo.data;
	if ((get_time() - philo.last_meal) > data->tt_die)
	{
		print_message(&philo, "died");
		release_sema(data->dead_lock);
		data->dead_flag = 1;
		return (1);
	}
	return (0);
}

// int	chk_dead(t_data *data)
// {
// 	int		temp;

// 	data->dead_lock = open_take(SEMA_DEATH);
// 	temp = data->dead_flag;
// 	release_sema(data->dead_lock);
// 	return (temp);
// }

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
