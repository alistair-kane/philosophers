/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alistair <alistair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 23:26:03 by alistair          #+#    #+#             */
/*   Updated: 2022/04/30 12:54:07 by alistair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

uint16_t	convert(long temp_l, int bytes);

int	data_init(t_data *data, char **argv, uint8_t i)
{
	data->tt_die = convert(ft_atoi(argv[2]), 16);
	data->tt_eat = convert(ft_atoi(argv[3]), 16);
	data->tt_sleep = convert(ft_atoi(argv[4]), 16);
	if (data->tt_die == 0 || data->tt_eat == 0|| data->tt_sleep == 0)
		return (0);
	// need to check if optional arg is passed
	if (argv[5] != NULL)
	{
		data->min_eat = convert(ft_atoi(argv[5]), 8);
		if (data->min_eat == 0)
			return (0);		
	}
	else
		data->min_eat = UINT8_MAX;
	data->id = i;
	return (1);
}

void	print_message(t_data *data, char *msg)
{
	struct timeval	tv;
	long long milliseconds;

	pthread_mutex_lock(data->print_lock);
	gettimeofday(&tv, NULL);
	milliseconds = tv.tv_sec*1000LL + tv.tv_usec/1000;
	printf("%lld %d %s \n", milliseconds, data->id, msg);
	pthread_mutex_unlock(data->print_lock);
}

void	*philosopher(void *arg)
{
	t_data		*data;
	int			left_fork;
	int			right_fork;
	uint8_t		meals;
	
	struct 		timeval	tv;
	long long	last_meal;
	long long	t_current;
	data = (t_data *)arg;
	
	int	tte;
	int	tts;
	int	ttd;
	tte = (int)data->tt_eat;
	tts = (int)data->tt_sleep;
	ttd = (int)data->tt_die;
	int i = data->id;
	pthread_mutex_t	*left_lock;
	pthread_mutex_t	*right_lock;

	gettimeofday(&tv, NULL);
	last_meal = tv.tv_sec*1000LL + tv.tv_usec/1000;
	t_current = tv.tv_sec*1000LL + tv.tv_usec/1000;
	meals = 0;
	if (i > 0)
	{
		if ((i + 1) != data->n)
			right_lock = &data->mutex_array[i + 1];
		else
			right_lock = &data->mutex_array[0];
	}
	left_lock = &data->mutex_array[i];
	while (1)
	{
		left_fork = 0; // 0 means not held
		right_fork = 0;
		if (!pthread_mutex_lock(left_lock))
		{
			print_message(data, "has taken a fork [L]");
			left_fork = 1;
		}
		if (!pthread_mutex_lock(right_lock))
		{
			print_message(data, "has taken a fork [R]");
			right_fork = 1;
		}
		if (left_fork && right_fork)
		{
			gettimeofday(&tv, NULL);
			last_meal = tv.tv_sec*1000LL + tv.tv_usec/1000;
			print_message(data, "is eating");
			usleep(tte * 1000);
			left_fork = pthread_mutex_unlock(left_lock);
			right_fork = pthread_mutex_unlock(right_lock);
			meals++;
			if (meals == data->min_eat)
			{
				print_message(data, "IS FULL!!");
				break ;
			}
			print_message(data, "is sleeping");
			usleep(tts * 1000);
		}
		print_message(data, "is thinking");
		gettimeofday(&tv, NULL);
		t_current = tv.tv_sec*1000LL + tv.tv_usec/1000;
		if (t_current - last_meal > ttd)
		{
			print_message(data, "died");
			break ;
		}
	}


	// printf("ID: [%d]\n", data->id);
	// printf("ttd: [%d]\n", data->tt_die);
	// printf("tte: [%d]\n", data->tt_eat);
	// printf("tts: [%d]\n", data->tt_sleep);
	// printf("min eat: [%d]\n", data->min_eat);
	// print_message(data);
	return NULL;
}

uint16_t	convert(long temp_l, int bytes)
{
	if (bytes == 16)
	{
		if (temp_l > UINT16_MAX || temp_l < 1)
			return (0);
		return((uint16_t)temp_l);
	}
	if (bytes == 8)
	{
		if (temp_l > UINT8_MAX || temp_l < 1)
			return (0);
		return((uint8_t)temp_l);
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_data			*data;
	pthread_t		*id_array;
	// pthread_mutex_t temp;
	pthread_mutex_t	*mutex_array;
	long		n; // max is 255, long because of atol
	int			i;

	if (argc >= 5 && argc <= 6)
	{
		n = ft_atoi(argv[1]);
		if (n > 255 || n < 1)
			return 1;

		data = malloc(sizeof(t_data) * n);
		data[0] = NULL;
		if (!data)
			return 1;
			

		// if (data_init(data, argv, 0) == 0)
		// 	return 1;

		
		// if (!data->mutex_array)
		// 	return 1;
		// data->n = n;
		id_array = malloc(sizeof(pthread_t *) * n);
		if (!id_array)
			return 1;


		mutex_array = malloc(sizeof(pthread_mutex_t) * n);
		i = -1;
		while (++i < n)
		{
			mutex_array[i] = malloc(sizeof(pthread_mutex_t));
			pthread_mutex_init(mutex_array[i], NULL);
		}
		
		i = -1;
		// init the threads for the philosopher function
		while (++i < n)
		{
			data_init(&data[i], argv, i, mutex_array);
			pthread_create(&id_array[i], NULL, philosopher, (void *)&data[i]);
		}
		pthread_mutex_init(data->print_lock, NULL);
		
		i = -1;
		while (++i < n)
			pthread_join(id_array[i], NULL);
		printf("after");
		
		pthread_mutex_destroy(data->print_lock);
		i = -1;
		while (++i < n)
			pthread_mutex_destroy(&data->mutex_array[i]);
		
		free(id_array);
		free(data);
		pthread_exit(NULL);
		return (0);
	}
	printf("error\n");
	return 0;
}