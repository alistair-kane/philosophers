/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alistair <alistair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 23:26:03 by alistair          #+#    #+#             */
/*   Updated: 2022/06/08 23:12:58 by alistair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
		if (data->philos[i].pid < 0) // fork failed
		{
			perror("Fork failed");
			exit(1);
		}
		else if (data->philos[i].pid == 0) // in child
			return (ph_func(&data->philos[i]));
	}
}

int	main(int argc, char *argv[])
{
	t_data		data;
	pthread_t	few;
	pthread_t	fw;

	if (set_table(&data, argc, argv) == 1)
		return (1);
	start_dinner(&data);
	if (data.n_meal > 0)
	{
		pthread_create(&few, NULL, finish_eating_waiter, &data);
		pthread_detach(few);		
	}
	pthread_create(&fw, NULL, finish_waiter, &data);
	pthread_detach(fw);
	tidy_up(&data);
	return (0);
}
