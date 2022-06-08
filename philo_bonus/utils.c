/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alistair <alistair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 11:18:31 by alkane            #+#    #+#             */
/*   Updated: 2022/06/08 23:11:26 by alistair         ###   ########.fr       */
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

void	print_message(t_philo *philo, char *msg)
{
	t_data	*data;

	data = philo->data;
	sem_wait(data->print_lock);
	printf("%lld\t%d\t%s\n", get_time() - data->start_ts, philo->id + 1, msg);
	sem_post(data->print_lock); // release the print semaphore
}

void	spend_time(long long time_ms)
{
	long long	end;

	end = get_time() + time_ms;
	while (get_time() < end)
		usleep(time_ms / 1000);
}

void	tidy_up(t_data *data)
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