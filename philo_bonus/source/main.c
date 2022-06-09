/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 23:26:03 by alistair          #+#    #+#             */
/*   Updated: 2022/06/09 12:39:45 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

// place where the forking can be done instead of pthread create

static int	set_table(t_data *data, int argc, char **argv)
{
	if (check_input(argc, argv) == 1)
		return (1);
	data->n_philo = ft_atoi(argv[1]);
	data->tt_die = ft_atoi(argv[2]);
	data->tt_eat = ft_atoi(argv[3]);
	data->tt_sleep = ft_atoi(argv[4]);
	if (argv[5] != NULL)
		data->n_meal = ft_atoi(argv[5]);
	else
		data->n_meal = -1;
	philo_init(data);
	return (0);
}

static void	start_dinner(t_data *data)
{
	int	i;

	data->start_ts = get_time();
	i = -1;
	while (++i < data->n_philo)
	{
		data->philos[i].last_meal = data->start_ts;
		data->philos[i].pid = fork();
		if (data->philos[i].pid < 0)
		{
			perror("Fork failed");
			exit(1);
		}
		else if (data->philos[i].pid == 0)
			return (ph_func(&data->philos[i]));
	}
}

static void	tidy_up(t_data *data)
{
	int	i;
	int	status;

	i = -1;
	while (++i < data->n_philo)
	{
		waitpid(data->philos[i].pid, &status, 0);
		sem_close(data->philos[i].checking);
		free(data->philos[i].name);
	}
	sem_close(data->done_lock);
	sem_close(data->print_lock);
	sem_close(data->forks);
	sem_close(data->finished);
}

int	main(int argc, char *argv[])
{
	t_data		data;
	pthread_t	waiter;

	if (set_table(&data, argc, argv) == 1)
		return (1);
	start_dinner(&data);
	if (data.n_meal > 0)
	{
		pthread_create(&waiter, NULL, finish_eating_waiter, &data);
		pthread_detach(waiter);
	}
	pthread_create(&waiter, NULL, finish_waiter, &data);
	pthread_detach(waiter);
	tidy_up(&data);
	return (0);
}
