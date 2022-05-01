/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 23:26:03 by alistair          #+#    #+#             */
/*   Updated: 2022/05/01 19:58:01 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

uint16_t	convert(long temp_l, int bytes);

int	check_input(char **argv)
{
	uint16_t	tt_die;
	uint16_t	tt_eat;
	uint16_t	tt_sleep;

	if (ft_atoi(argv[1]) > 255 || ft_atoi(argv[1]) < 1)
		return (1);
	tt_die = convert(ft_atoi(argv[2]), 16);
	tt_eat = convert(ft_atoi(argv[3]), 16);
	tt_sleep = convert(ft_atoi(argv[4]), 16);
	if (tt_die == 0 || tt_eat == 0|| tt_sleep == 0)
		return (1);
	// need to check if optional arg is passed
	if (argv[5] != NULL)
	{
		if (convert(ft_atoi(argv[5]), 8) == 0)
			return (1);
	}
	return (0);
}

t_data	data_init(t_data data, char **argv, int i, pthread_mutex_t *mutex_array, int n)
{
	data.tt_die = convert(ft_atoi(argv[2]), 16);
	data.tt_eat = convert(ft_atoi(argv[3]), 16);
	data.tt_sleep = convert(ft_atoi(argv[4]), 16);
	// need to check if optional arg is passed
	if (argv[5] != NULL)
		data.min_eat = convert(ft_atoi(argv[5]), 8);
	else
		data.min_eat = UINT8_MAX;
	data.id = (uint8_t)i;
	if (n == 1)
	{
		data.left_fork = &mutex_array[0];
		data.right_fork = NULL;
	}
	else
	{
		if ((i + 1) != (n - 1))
			data.right_fork = &mutex_array[i + 1];
		else
			data.right_fork = &mutex_array[0];
		data.left_fork = &mutex_array[i];
	}
	return (data);
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

	gettimeofday(&tv, NULL);
	last_meal = tv.tv_sec*1000LL + tv.tv_usec/1000;
	t_current = tv.tv_sec*1000LL + tv.tv_usec/1000;
	meals = 0;
	while (1)
	{
		left_fork = 0; // 0 means not held
		right_fork = 0;
		if (!pthread_mutex_lock(data->left_fork))
		{
			print_message(data, "has taken a fork [L]");
			left_fork = 1;
		}
		if (data->right_fork != NULL && !pthread_mutex_lock(data->right_fork))
		{
			print_message(data, "has taken a fork [R]");
			right_fork = 1;
		}
		if (left_fork && right_fork)
		{
			gettimeofday(&tv, NULL);
			last_meal = tv.tv_sec * 1000LL + tv.tv_usec / 1000;
			print_message(data, "is eating");
			usleep(tte * 1000);
			left_fork = pthread_mutex_unlock(data->left_fork);
			right_fork = pthread_mutex_unlock(data->right_fork);
			meals++;
			if (meals == data->min_eat)
			{
				print_message(data, "IS FULL!!");
				break ;
			}
			print_message(data, "is sleeping");
			usleep(tts * 1000);
		}
		if (left_fork)
			left_fork = pthread_mutex_unlock(data->left_fork);
		if (right_fork)
			right_fork = pthread_mutex_unlock(data->right_fork);
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
	return (NULL);
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
	pthread_t		*thread_array;
	pthread_mutex_t	*mutex_array;
	pthread_mutex_t	print_lock;
	long			n; // max is 255, long because of atol
	int				i;

	if (argc >= 5 && argc <= 6)
	{
		if (check_input(argv) == 1)
			return (1);
		n = ft_atoi(argv[1]);
		// allocate the memory for each data struct to be later passed to each thread
		// i.e. initialization happens later
		data = ft_calloc(sizeof(t_data) , n);
		if (!data)
			return 1;

		// allocate memory for all the threads
		thread_array = malloc(sizeof(pthread_t) * n);
		if (!thread_array)
			return 1;

		// allocate the memory to hold all the mutex
		mutex_array = malloc(n * sizeof(pthread_mutex_t));
		i = -1;
		while (++i < n)
			pthread_mutex_init(&mutex_array[i], NULL);
		
		pthread_mutex_init(&print_lock, NULL);

		// init the threads for the philosopher function
		i = -1;
		while (++i < n)
		{
			data[i].print_lock = &print_lock;
			data[i] = data_init(data[i], argv, i, mutex_array, n);
			pthread_create(&thread_array[i], NULL, philosopher, &data[i]);
		}
		i = -1;
		while (++i < n)
			pthread_join(thread_array[i], NULL);
		
		pthread_mutex_destroy(&print_lock);
		i = -1;
		while (++i < n)
			pthread_mutex_destroy(&mutex_array[i]);
		
		free(thread_array);
		free(data);
		pthread_exit(NULL);
		return (0);
	}
	printf("error\n");
	return 0;
}