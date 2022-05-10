/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 17:04:29 by alkane            #+#    #+#             */
/*   Updated: 2022/05/10 18:21:45 by alkane           ###   ########.fr       */
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

typedef struct s_philo
{

	int			id;
	int			n_eaten;
	int			left_fork;
	int			right_fork;
	long long	last_meal;
	t_data		*data;
	pthread_t	thread_id;
}				t_philo;

typedef struct s_data
{
	long			n_philo; // max is 255, long because of atol
	int				tt_die;
	int				tt_eat;
	int				tt_sleep;
	int				all_eaten;
	int				n_meal;
	int				dead_flag;
	long long		start_ts;
	pthread_mutex_t	*fork_array;
	pthread_mutex_t	*print_lock;
	pthread_mutex_t	*meal_lock;
	t_philo			*philos;
}				t_data;

long	ft_atoi(const char *nptr);
void	*ft_calloc(size_t nmemb, size_t size);

int			set_table(t_data *data, int argc, char **argv);
long long	get_time(void);
void		print_message(t_philo *philo, char *msg);
void		*philosopher(void *arg);
void		clear_table(t_data *data);
void		check_dead(t_data *data, t_philo philo);
void		sleep_or_die(t_philo *philo, int sleep, long long last_meal);

#endif