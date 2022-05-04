/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 11:14:51 by alkane            #+#    #+#             */
/*   Updated: 2022/05/04 13:34:36 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static uint16_t	convert(long temp_l, int bytes)
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

int	check_input(int	argc, char **argv)
{
	uint16_t	tt_die;
	uint16_t	tt_eat;
	uint16_t	tt_sleep;
	if (argc >= 5 && argc <= 6)
	{
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
	}
	else
		return (1);
	return (0);
}

void	assign_mutexs(t_data *data, t_philo philo)
{
	if (data->n == 1)
	{
		philo.left_mutex = &data->mutex_array[0];
		philo.right_mutex = NULL;
	}
	else
	{
		if ((philo.id + 1) < (data->n - 1))
			philo.right_mutex = &data->mutex_array[philo.id + 1];
		else
			philo.right_mutex = &data->mutex_array[0];
		philo.left_mutex = &data->mutex_array[philo.id];
	}
}

void	philo_init(t_data *data, t_philo philo, char **argv, int i)
{
	philo.id = (uint8_t)i;
	philo.print_lock = data->print_lock;
	assign_mutexs(data, philo);
	philo.tt_die = convert(ft_atoi(argv[2]), 16);
	philo.tt_eat = convert(ft_atoi(argv[3]), 16);
	philo.tt_sleep = convert(ft_atoi(argv[4]), 16);
	// need to check if optional arg is passed
	if (argv[5] != NULL)
		philo.min_eat = convert(ft_atoi(argv[5]), 8);
	else
		philo.min_eat = UINT8_MAX;
	// philo.left_fork = 0; set by calloc
	// philo.right_fork = 0; set by calloc
	philo.start_time = get_time();
}

void	set_table(t_data *data, char **argv)
{
	int				i;

	data = malloc(sizeof(t_data));
	if (!data)
		return ;
	data->n = ft_atoi(argv[1]);
	data->philos = ft_calloc(sizeof(t_philo), data->n);
	if (!data->philos)
		return ;

	// allocate memory for all the threads
	data->thread_array = malloc(sizeof(pthread_t) * data->n);
	if (!data->thread_array)
		return ;
	// allocate the memory to hold all the mutex
	data->mutex_array = malloc(sizeof(pthread_mutex_t) * data->n);
	i = -1;
	while (++i < data->n)
		pthread_mutex_init(&data->mutex_array[i], NULL);
	data->print_lock = malloc(sizeof(pthread_mutex_t *));
	pthread_mutex_init(data->print_lock, NULL);
	// init the threads for the philosopher function
	i = -1;
	while (++i < data->n)
	{
		philo_init(data, data->philos[i], argv, i);
		pthread_create(&data->thread_array[i], NULL, philosopher, &data->philos[i]);
	}
	i = -1;
	while (++i < data->n)
		pthread_join(data->thread_array[i], NULL);
}