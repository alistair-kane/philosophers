/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alistair <alistair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 19:04:09 by alkane            #+#    #+#             */
/*   Updated: 2022/06/02 07:14:15 by alistair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	chk_philo_death(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	if (philo->eating == 0 && get_time() > philo->limit)
	{
		print_message(philo, "died", 1);
		pthread_mutex_lock(&(data->print_lock));
		pthread_mutex_unlock(&(data->done_lock));
		return (1);
	}
	return (0);
}
