/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 23:26:03 by alistair          #+#    #+#             */
/*   Updated: 2022/05/03 00:35:57 by alkane           ###   ########.fr       */
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

void	assign_mutexs(t_data *data, int i, pthread_mutex_t *mutex_array, int n)
{
	data->id = (uint8_t)i;
	if (n == 1)
	{
		data->left_mutex = &mutex_array[0];
		data->right_mutex = NULL;
	}
	else
	{
		if ((i + 1) < (n - 1))
			data->right_mutex = &mutex_array[i + 1];
		else
			data->right_mutex = &mutex_array[0];
		data->left_mutex = &mutex_array[i];
	}
}
	
t_data	data_init(t_data data, char **argv, long long milliseconds)
{
	data.tt_die = convert(ft_atoi(argv[2]), 16);
	data.tt_eat = convert(ft_atoi(argv[3]), 16);
	data.tt_sleep = convert(ft_atoi(argv[4]), 16);
	// need to check if optional arg is passed
	if (argv[5] != NULL)
		data.min_eat = convert(ft_atoi(argv[5]), 8);
	else
		data.min_eat = UINT8_MAX;
	data.left_fork = 0;
	data.right_fork = 0;
	data.start_time = milliseconds;
	return (data);
}

void	print_message(t_data *data, char *msg)
{
	struct timeval	tv;
	long long		milliseconds;

	pthread_mutex_lock(data->print_lock);
	gettimeofday(&tv, NULL);
	milliseconds = tv.tv_sec*1000LL + tv.tv_usec/1000;
	milliseconds = milliseconds - data->start_time;
	printf("%lld %d %s \n", milliseconds, data->id + 1, msg);
	pthread_mutex_unlock(data->print_lock);
}

int	check_dead(t_data *data, long long last_meal)
{
	struct 		timeval	tv;
	long long	t_current;

	gettimeofday(&tv, NULL);
	t_current = tv.tv_sec * 1000LL + tv.tv_usec / 1000;
	if (t_current - last_meal > data->tt_die)
	{
		print_message(data, "died after eating");
		return (1);
	}
	return (0);
}

long long	update_last_meal(void)
{
	struct 		timeval	tv;
	long long	last_meal;
	
	gettimeofday(&tv, NULL);
	last_meal = tv.tv_sec*1000LL + tv.tv_usec/1000;
	return (last_meal);
}

void	pick_up_fork(t_data *data, char c)
{
	if (c == 'L')
	{
		pthread_mutex_lock(data->left_mutex);
		print_message(data, "has taken a fork [L]");
		data->left_fork = 1;
		pthread_mutex_unlock(data->left_mutex);
	}
	else
	{
		pthread_mutex_lock(data->right_mutex);
		print_message(data, "has taken a fork [R]");
		data->right_fork = 1;
		pthread_mutex_unlock(data->right_mutex);
	}
}

int	eat_sleeping(t_data *data, uint8_t *meals)
{
	long long	last_meal;

	last_meal = update_last_meal();
	print_message(data, "is eating");
	usleep((int)data->tt_eat * 1000); // could not die here because started eating
	pthread_mutex_lock(data->left_mutex); 
	data->left_fork = pthread_mutex_unlock(data->left_mutex); // unlock left fork
	pthread_mutex_lock(data->right_mutex); 
	data->right_fork = pthread_mutex_unlock(data->right_mutex); // unlock right fork
	(*meals)++;
	if (check_dead(data, last_meal))
		return (1);
	if (*meals == data->min_eat)
	{
		print_message(data, "IS FULL!!");
		return (1);
	}
	print_message(data, "is sleeping");
	usleep((int)data->tt_sleep * 1000);
	return (0);
}

void	*philosopher(void *arg)
{
	t_data		*data;
	uint8_t		meals;
	long long	last_meal;

	data = (t_data *)arg;
	meals = 0;
	while (1)
	{
		last_meal = update_last_meal();
		if (!data->left_fork)
			pick_up_fork(data, 'L');
		if (data->left_fork && data->right_mutex != NULL && !data->right_fork)
			pick_up_fork(data, 'R');
		if (data->left_fork && data->right_fork)
		{
			if (eat_sleeping(data, &meals))
				break ;
		}
		print_message(data, "is thinking");
		if (check_dead(data, last_meal))
			break ;
	}
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
	struct timeval	tv;
	long long		milliseconds;

	if (argc >= 5 && argc <= 6)
	{
		if (check_input(argv) == 1)
			return (1);
		n = ft_atoi(argv[1]);
		// allocate the memory for each data struct to be later passed to each thread
		// i.e. initialization happens later
		data = malloc(sizeof(t_data) * n);
		if (!data)
			return 1;

		// allocate memory for all the threads
		thread_array = malloc(sizeof(pthread_t) * n);
		if (!thread_array)
			return 1;

		// allocate the memory to hold all the mutex
		mutex_array = malloc(sizeof(pthread_mutex_t) * n);
		i = -1;
		while (++i < n)
			pthread_mutex_init(&mutex_array[i], NULL);
		
		pthread_mutex_init(&print_lock, NULL);
		gettimeofday(&tv, NULL);
		// start_time to fix time printout
		milliseconds = tv.tv_sec*1000LL + tv.tv_usec/1000;

		// init the threads for the philosopher function
		i = -1;
		while (++i < n)
		{
			// data[i].death_flag = &death_flag;
			data[i] = data_init(data[i], argv, milliseconds);
			data[i].print_lock = &print_lock;
			assign_mutexs(&data[i], i, mutex_array, n);
			pthread_create(&thread_array[i], NULL, philosopher, &data[i]);
		}
		i = -1;
		while (++i < n)
			pthread_join(thread_array[i], NULL);

		pthread_mutex_destroy(&print_lock);
		i = -1;
		while (++i < n)
			pthread_mutex_destroy(&mutex_array[i]);
		free(data);
		free(thread_array);
		free(mutex_array);
		return (0);
	}
	printf("error\n");
	return 0;
}