/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 11:18:31 by alkane            #+#    #+#             */
/*   Updated: 2022/05/21 19:02:14 by alkane           ###   ########.fr       */
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

sem_t	*open_sem(char *sem_name, int oflags, mode_t mode, int n)
{
	sem_t	*semaphore;

	semaphore = sem_open(sem_name, oflags, mode, n);
	if (semaphore == SEM_FAILED) 
	{
		printf("%s\n",sem_name);
        perror("sem_open(3) failed");
        exit(EXIT_FAILURE);
    }
	return (semaphore);
}

void	take_sem(sem_t *semaphore)
{
	if (sem_wait(semaphore) < 0) // take the semaphore
	{
		perror("sem_wait(3) failed");
		exit(EXIT_FAILURE);
	}
}

void	release_sema(sem_t	*semaphore)
{
	if (sem_post(semaphore) < 0) // release the semaphore
	{
		perror("sem_post(3) error on child");
		exit(EXIT_FAILURE);
	}
}

void	print_message(t_philo *philo, char *msg)
{
	t_data	*data;

	data = philo->data;
	// pthread_mutex_lock(&(data->print_lock)); no longer exists
	if (chk_dead_flag(data) == 0)
	{
		take_sem(data->print_lock);
		printf("%lld %d %s \n", get_time() - data->start_ts, philo->id + 1, msg);
		release_sema(data->print_lock); // release the print semaphore
	}
	// pthread_mutex_unlock(&(data->print_lock));
}

void	spend_time(t_philo philo, long long stuff_time)
{
	long long	start;

	start = get_time();
	while (chk_philo_death(philo) == 0)
	{
		if (get_time() - start >= stuff_time)
			break ;
		usleep(50);
	}
}
