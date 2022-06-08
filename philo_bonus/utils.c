/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 11:18:31 by alkane            #+#    #+#             */
/*   Updated: 2022/06/08 13:53:48 by alkane           ###   ########.fr       */
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
	sem_wait(data->print_lock);
	printf("%lld %d %s \n", get_time() - data->start_ts, philo->id + 1, msg);
	sem_post(data->print_lock); // release the print semaphore
}

void	spend_time(long long time_ms)
{
	long long	end;

	end = get_time() + time_ms;
	while (get_time() < end)
		usleep(time_ms / 1000);
}

int	tidy_up(t_data *data)
{
	int	i;
	int	status;

	i = 0;
	while (i < data->n_philo)
	{
		waitpid(data->philos[i].pid, &status, 0);
		sem_close(data->philos[i].checking);
		free(data->philos[i].name);
	}
	sem_close(data->done_lock);
	sem_close(data->print_lock);
	sem_close(data->forks);
	sem_close(data->finished);
	return (0);
}