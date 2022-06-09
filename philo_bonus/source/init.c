/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 11:14:51 by alkane            #+#    #+#             */
/*   Updated: 2022/06/09 12:39:45 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static sem_t	*open_sem(char *sem_name, int n)
{
	sem_t	*semaphore;
	mode_t	mode;

	mode = (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
	semaphore = sem_open(sem_name, O_CREAT | O_EXCL, mode, n);
	if (semaphore != SEM_FAILED)
		return (semaphore);
	sem_unlink(sem_name);
	return (sem_open(sem_name, O_CREAT | O_EXCL, mode, n));
}

static int	convert(long temp_l)
{
	if (temp_l > INT32_MAX || temp_l < 1)
		return (0);
	return ((int)temp_l);
}

static char	*create_name(int id)
{
	char	*id_str;
	char	*ret;

	id_str = ft_itoa(id);
	ret = ft_strjoin(PHILO_PREFIX, id_str);
	free(id_str);
	return (ret);
}

int	check_input(int argc, char **argv)
{
	int	tt_die;
	int	tt_eat;
	int	tt_sleep;

	if (argc >= 5 && argc <= 6)
	{
		if (ft_atoi(argv[1]) > 250 || ft_atoi(argv[1]) < 1)
			return (1);
		tt_die = convert(ft_atoi(argv[2]));
		tt_eat = convert(ft_atoi(argv[3]));
		tt_sleep = convert(ft_atoi(argv[4]));
		if (tt_die == 0 || tt_eat == 0 || tt_sleep == 0)
			return (1);
		if (argv[5] != NULL)
		{
			if (convert(ft_atoi(argv[5])) == 0)
				return (1);
		}
		return (0);
	}
	return (1);
}

void	philo_init(t_data *data)
{
	int	i;

	data->done_lock = open_sem(SEMA_DONE, 0);
	data->print_lock = open_sem(SEMA_PRINT, 1);
	data->forks = open_sem(SEMAFORKS, data->n_philo + 1);
	data->finished = open_sem(SEMA_FINISHED, 0);
	i = -1;
	while (++i < data->n_philo)
	{
		data->philos[i].name = create_name(i);
		data->philos[i].checking = open_sem(data->philos[i].name, 1);
		data->philos[i].id = i;
		data->philos[i].data = data;
	}
}

/*
In the Bonus. 
Every Philosopher has its own thread to check the lifetime.
Because the Philosopher process is waiting for the fork semaphores.  
He can't check its lifetime while waiting. 
So it is outsourced.
*/