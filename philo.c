/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alistair <alistair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 23:26:03 by alistair          #+#    #+#             */
/*   Updated: 2022/04/02 23:50:31 by alistair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_data	*data_init(long tt_die, long tt_eat, long tt_sleep, long min_eat)
{
	t_data *data;

	data = malloc(sizeof(t_data *));
	// input checking needed?
	data->tt_die = tt_die;
	data->tt_eat = tt_eat;
	data->tt_sleep = tt_sleep;
	data->min_eat = min_eat;
}

void	*philosopher(void *vargp)
{

}

int	main(int argc, char *argv[])
{
	t_data		*data;
	long		n;
	int			i;
	pthread_t	*id_array;
	int			*index;

	index = NULL;
	if (argc >= 4 && argc <= 5)
	{
		n = ft_atoi(argv[1]);
		// need to check if optional arg is passed
		data = data_init(ft_atoi(argv[2]), ft_atoi(argv[3]), ft_atoi(argv[4]));// ft_atoi(argv[5])
		i = 0;
		index = ft_calloc(n, sizeof(int));
		while (i++ < n)
			index[i] = i;
		id_array = malloc(sizeof(pthread_t *) * n);
		i = 0;
		// init the threads for the philosopher function
		while (i++ < n)
			pthread_create(&id_array[i], NULL, philosopher, (void *)&index[i]);
		i = 0;
		while (i++ < n)
			pthread_join(id_array[i], NULL);
	// 	ttd = ft_atoi(argv[2]);
	// 	tte = ft_atoi(argv[3]);
	// 	tts = ft_atoi(argv[4]);
	}
	return 0;
}