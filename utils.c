/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 11:18:31 by alkane            #+#    #+#             */
/*   Updated: 2022/05/10 18:17:27 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(void)
{
	struct timeval	tv;
	long long		milliseconds;

	gettimeofday(&tv, NULL);
	milliseconds = tv.tv_sec * 1000LL + tv.tv_usec / 1000;
	return (milliseconds);
}

void	print_message(t_philo *philo, char *msg)
{
	pthread_mutex_lock(philo->print_lock);
	printf("%lld %d %s \n", get_time() - philo->start_time, philo->id + 1, msg);
	pthread_mutex_unlock(philo->print_lock);
}

// int	check_dead(t_philo *philo, long long last_meal)
// {
// 	if ((get_time() - last_meal) > philo->tt_die)
// 	{
// 		print_message(philo, "died");
		
// 		pthread_mutex_lock(philo->dead_lock);
// 		*(philo->dead_flag) = 1;
// 		pthread_mutex_unlock(philo->dead_lock);
		
// 		return (1);
// 	}
// 	return (0);
// }

void	check_dead(t_data *data, t_philo philo)
{
	if ((get_time() - philo.last_meal) > data->tt_die)
	{
		print_message(&philo, "died");
		data->dead_flag = 1;
	}
}

void	sleep_or_die(t_philo *philo, long long last_meal)
{
	int	sleep;
	
	sleep = (int)philo->tt_sleep;
	while (sleep > 0)
	{
		usleep(1000); // sleep for 1 ms
		// if (check_dead(philo, last_meal) == 1)
			// return(1);
		check_dead(philo, last_meal);
		sleep -= 1; // take 1 ms from the time
	}
	// return (0);
}