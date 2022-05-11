/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 11:18:31 by alkane            #+#    #+#             */
/*   Updated: 2022/05/11 17:48:55 by alkane           ###   ########.fr       */
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
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&(data->print_lock));
	// if (check_df(data) == 0)
		printf("%lld %d %s \n", get_time() - data->start_ts, philo->id + 1, msg);
	pthread_mutex_unlock(&(data->print_lock));
}

void	check_dead(t_philo philo)
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
	usleep(100); // why sleep here?
}

int	check_df(t_data *data)
{
	int	temp;
	
	pthread_mutex_lock(&(data->dead_lock));
	temp = data->dead_flag;
	pthread_mutex_unlock(&(data->dead_lock));
	return (temp);
}

int chk_phm(t_philo philo)
{
	t_data	*data;
	int		temp;

	data = philo.data;
	pthread_mutex_lock(&(data->eat_lock));
	temp = philo.n_eaten;
	pthread_mutex_unlock(&(data->eat_lock));
	return (temp);
}

int	check_total_eat(t_data *data)
{
	int	temp;
	
	pthread_mutex_lock(&(data->eat_lock));
	temp = data->all_eaten;
	pthread_mutex_unlock(&(data->eat_lock));
	return (temp);
}

void	do_stuff(t_data *data, long long stuff_time)
{
	long long	start;

	start = get_time();
	while (check_df(data) == 0)
	{
		if (get_time() - start >= stuff_time)
			break ;
		usleep(50);
	}
}
