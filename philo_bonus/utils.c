/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 11:18:31 by alkane            #+#    #+#             */
/*   Updated: 2022/05/14 11:44:37 by alkane           ###   ########.fr       */
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

sem_t	*open_take(char *sem_name)
{
	sem_t	*semaphore;

	semaphore = sem_open(sem_name, O_RDWR);
	if (semaphore == SEM_FAILED) 
	{
        perror("sem_open(3) failed");
        exit(EXIT_FAILURE);
    }
	if (sem_wait(semaphore) < 0) // take the semaphore
	{
		perror("sem_wait(3) failed on print sema");
		exit(EXIT_FAILURE);
	}
	return (semaphore);
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
	data->print_lock = open_take(SEMA_PRINT);
	// pthread_mutex_lock(&(data->print_lock)); no longer exists
	if (data->dead_flag == 0)
		printf("%lld %d %s \n", get_time() - data->start_ts, philo->id + 1, msg);
	release_sema(data->print_lock); // release the print semaphore
	// pthread_mutex_unlock(&(data->print_lock));
}

void	spend_time(t_data *data, long long stuff_time)
{
	long long	start;

	start = get_time();
	while (chk_dead(data) == 0)
	{
		if (get_time() - start >= stuff_time)
			break ;
		usleep(50);
	}
}
