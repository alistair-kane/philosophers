/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 17:04:29 by alkane            #+#    #+#             */
/*   Updated: 2022/05/01 18:05:56 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h> 
# include <stdint.h>
# include <stdlib.h>
# include <sys/time.h>
# include <time.h>
# include <unistd.h>

typedef struct s_data
{
	int	n;

	uint16_t	tt_die;
	uint16_t	tt_eat;
	uint16_t	tt_sleep;
	uint8_t		min_eat;
	uint8_t		id;
	// pthread_mutex_t	*mutex_array;
	pthread_mutex_t	*print_lock;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	


	pthread_mutex_t *forkzero;
}				t_data;

long	ft_atoi(const char *nptr);
void	*ft_calloc(size_t nmemb, size_t size);

#endif