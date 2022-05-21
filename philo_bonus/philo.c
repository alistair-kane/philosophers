/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 23:26:03 by alistair          #+#    #+#             */
/*   Updated: 2022/05/14 12:05:58 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include "errno.h"

// void	eating(t_philo *philo)
// {
// 	t_data	*data;

// 	data = philo->data;
// 	pthread_mutex_lock(&(data->dead_lock));
// 	philo->last_meal = get_time();
// 	pthread_mutex_unlock(&(data->dead_lock));
// 	pthread_mutex_lock(&(data->eat_lock));
// 	(philo->n_eaten)++;
// 	pthread_mutex_unlock(&(data->eat_lock));
// 	print_message(philo, "is eating");
// 	spend_time(data, data->tt_eat);
// 	pthread_mutex_unlock(&(data->fork_array[philo->right_fork]));
// 	pthread_mutex_unlock(&(data->fork_array[philo->left_fork]));
// }

void	*ph_monitor(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	while (1)
	{
		data->thread_lock = open_take(SEMA_DEATH);
		if (chk_philo_death(*philo) == 1)
			break ;
		release_sema(data->thread_lock);
	}
	release_sema(data->thread_lock);
	return (NULL);
}

static void	ph_func_child(t_philo *philo)
{
	t_data		*data;
	pthread_t	alive_check;
	int		i;

	printf("I am child philo process #:[%d]\n",philo->id + 1);
	data = philo->data;
	data->semaforks = sem_open(SEMAFORK, O_RDWR);
    if (data->semaforks == SEM_FAILED)
	{
        perror("sem_open(3) failed");
        exit(EXIT_FAILURE);
    }
	pthread_create(&(alive_check), NULL, ph_monitor, (void *)&philo); // check return of pthread create?
	open_take(SEMA_THREAD);
	while (data->dead_flag == 0)
	{
		release_sema(data->thread_lock);
		// eating
		i = -1;
		while (++i < 2)
		{
			if (sem_wait(data->semaforks) < 0) 
			{
				perror("sem_wait(3) failed on child");
				continue;
			}
			print_message(philo, "has taken a fork");
		}
		// here both forks have been accquired
		spend_time(data, data->tt_eat);
		i = -1;
		while (++i < 2)
		{
			if (sem_post(data->semaforks) < 0)
			{
				perror("sem_post(3) error on child");
			}
			usleep(1000);
		}
	}




	// while (chk_dead(data) == 0)
	// {
	// 	eating(philo);
	// 	if (chk_total_eat(data) == 1)
	// 		break ;
	// 	print_message(philo, "is sleeping");
	// 	spend_time(data, data->tt_sleep);
	// 	print_message(philo, "is thinking");
	// }
	exit(0);
}

// place where the forking can be done instead of pthread create
static int	start_dinner(t_data *data)
{
	t_philo	*philo;
	pid_t	pid;
	// sem_t	*semafork;
	// sem_t	*death_lock;
	// sem_t	*print_fork;
	int		i;

	philo = data->philos;
	data->start_ts = get_time();
	sem_unlink(SEMAFORK);
	sem_unlink(SEMA_DEATH);
	sem_unlink(SEMA_PRINT);
	data->semaforks = sem_open(SEMAFORK, O_CREAT | O_EXCL, SEM_PERMS, data->n_philo);
	data->dead_lock = sem_open(SEMA_THREAD, O_CREAT | O_EXCL, SEM_PERMS, 1);
	data->dead_lock = sem_open(SEMA_DEATH, O_CREAT | O_EXCL, SEM_PERMS, 1);
	data->print_lock = sem_open(SEMA_PRINT, O_CREAT | O_EXCL, SEM_PERMS, 1);
	if ((data->semaforks == SEM_FAILED) || (data->dead_lock == SEM_FAILED \
		|| (data->print_lock == SEM_FAILED) || (data->thread_lock == SEM_FAILED)))
	{
		printf("semafail:%d\n", errno);
		exit(EXIT_FAILURE);
	}
	if (sem_close(data->semaforks) < 0) //closed in parent as we don't use it here
	{
        sem_unlink(SEMAFORK);
        exit(EXIT_FAILURE);
    }
	// if (data->n_philo == 1)
	// {
	// 	print_message(philo, "has taken a fork [L]");
	// 	return (NULL);
	// }
	i = -1;
	while (++i < data->n_philo)
	{
		pid = fork();
		if (pid < 0) // fork failed
			return (1);
		else if (pid == 0) // in child
			ph_func_child(&philo[i]);
	}
	i = -1;
	while (++i < data->n_philo)
		waitpid(-1, NULL, 0);
	
	printf("[parent:]\tall children returned!\n");
	// }
	// alive_loop(data, data->philos);
	// tidy_up(data, philo);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_data	data;
	if (set_table(&data, argc, argv) == 1)
		return (2);
	printf("din\n");
	if (start_dinner(&data) == 1)
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
