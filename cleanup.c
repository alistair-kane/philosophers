/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 12:19:11 by alkane            #+#    #+#             */
/*   Updated: 2022/05/10 15:51:14 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clear_table(t_data *data)
{
    int	i;

    // pthread_mutex_destroy(&(data->print_lock));
    // free(&data->print_lock);
	
	// pthread_mutex_destroy(&(data->dead_lock));
    // free(&data->dead_lock);
	
	i = -1;
	while (++i < data->n)
		pthread_mutex_destroy(&(data->fork_array[i]));
	free(data->fork_array);
	free(data->fork_status);
	free(data->thread_array);
    free(data->philos);
	free(data);
}
