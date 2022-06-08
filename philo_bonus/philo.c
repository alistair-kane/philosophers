/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 23:26:03 by alistair          #+#    #+#             */
/*   Updated: 2022/06/08 14:07:20 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	pick_up_forks(t_philo *philo)
{
	sem_wait(philo->data->forks);
	print_message(philo, "has taken a fork [L]");
	sem_wait(philo->data->forks);
	print_message(philo, "has taken a fork [R]");
}

void	eat(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	sem_wait(philo->checking);
	philo->last_meal = get_time();
	print_message(philo, "has taken a fork [R]");
	philo->n_eaten += 1;	
	if (philo->n_eaten == data->n_meal)
		sem_post(data->finished);
	spend_time(data->tt_eat);
	sem_post(data->forks);
	sem_post(data->forks);
	sem_post(philo->checking);
}

void	sleeping_thinking(t_philo *philo)
{
	print_message(philo, "is sleeping");
	spend_time(philo->data->tt_sleep);
	print_message(philo, "is thinking");
}

static void	ph_func(t_philo *philo)
{
	t_data		*data;
	pthread_t	alive_check;

	data = philo->data;
	pthread_create(&alive_check, NULL, monitor_thread, philo);
	if (philo->id % 2 == 0)
		usleep(data->tt_eat * 1000);
	while (1)
	{
		pick_up_forks(philo);
		eat(philo);
		sleeping_thinking(philo);
	}
	exit (0);
}

// place where the forking can be done instead of pthread create
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
	pthread_t	monitor;

	if (set_table(&data, argc, argv) == 1)
		return (1);
	start_dinner(&data);
	// back in parent
	if (data.n_meal > 0)
		pthread_create(&monitor, NULL, monitor_thread_eat, &data);
	pthread_create(&monitor, NULL, monitor_finish, &data);

	if (tidy_up(&data) == 1)
		return (3);
	return (0);
}


// static void	alive_loop(t_data *d, t_philo *p)
// {
// 	int	i;

// 	while (chk_total_eat(d) == 0)
// 	{
// 		i = -1;
// 		while (++i < d->n_philo && (chk_dead(d) == 0))
// 			chk_any_deaths(p[i]);
// 		if (chk_dead(d) == 1)
// 			break ;
// 		i = 0;
// 		while (d->n_meal != -1 && i < d->n_philo &&
// 			chk_ph_meals(p[i]) >= d->n_meal)
// 			i++;
// 		if (i == d->n_philo)
// 		{
// 			pthread_mutex_lock(&(d->eat_lock));
// 			d->all_eaten = 1;
// 			pthread_mutex_unlock(&(d->eat_lock));
// 		}
// 	}
// }

// static void	tidy_up(t_data *data, t_philo *philos)
// {
// 	int	i;

// 	i = -1;
// 	while (++i < data->n_philo)
// 		pthread_join(philos[i].thread_id, NULL);
// 	i = -1;
// 	while (++i < data->n_philo)
// 		pthread_mutex_destroy(&(data->fork_array[i]));
// 	pthread_mutex_destroy(&(data->print_lock));
// 	pthread_mutex_destroy(&(data->dead_lock));
// 	pthread_mutex_destroy(&(data->eat_lock));
// }
