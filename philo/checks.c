/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 19:04:09 by alkane            #+#    #+#             */
/*   Updated: 2022/06/07 00:23:53 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void    *monitor_thread(void *arg)
{
    t_philo *philo;
    // t_data  *data;
    long long   time;
    
    philo = arg;
    // data = philo->data;
    // int i = 0;
    while (philo->data->done_flag == 0)
    {
        pthread_mutex_lock(&(philo->checking));
        pthread_mutex_lock(&(philo->data->done_lock));
        time = get_time() - philo->last_meal;
        if (time >= philo->data->tt_die && philo->data->done_flag == 0)
        {
	        printf("%lld %d %s \n", get_time() - philo->data->start_t, 
                philo->id + 1, "died");
            philo->data->done_flag = 1;
        }
        pthread_mutex_unlock(&(philo->data->done_lock));
        pthread_mutex_unlock(&(philo->checking));
    }
    return (NULL);
}

void    *monitor_thread_eat(void *arg)
{
    t_data  *data;
    
    data = arg;
    while (data->done_flag == 0)
    {
        pthread_mutex_lock(&(data->done_lock));
        if (data->philos_done_eating == data->n_philo)
            data->done_flag = 1;
        pthread_mutex_unlock(&(data->done_lock));
    }
    return (NULL);
}
