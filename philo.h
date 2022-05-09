/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 17:04:29 by alkane            #+#    #+#             */
/*   Updated: 2022/05/09 17:18:56 by alkane           ###   ########.fr       */
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
	uint16_t		tt_die;
	uint16_t		tt_eat;
	uint16_t		tt_sleep;
	uint8_t			min_eat;
	
	int				id;
	
	int8_t			left_fork;
	int8_t			right_fork;
	
	pthread_mutex_t	*left_mutex;
	pthread_mutex_t	*right_mutex;
	pthread_mutex_t	*print_lock;
	pthread_mutex_t	*dead_lock;
	int				*dead_flag;
	long long		start_time;
}					t_philo;

typedef struct s_data
{
	long			n; // max is 255, long because of atol
	pthread_t		*thread_array;
	pthread_mutex_t	*mutex_array;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	dead_lock;
	int				dead_flag;
	t_philo			*philos;
}				t_data;

long	ft_atoi(const char *nptr);
void	*ft_calloc(size_t nmemb, size_t size);

// int		check_input(int	argc, char **argv);
int			set_table(t_data *data, int argc, char **argv);
// void		philo_init(t_data *data, t_philo *philo, char **argv, int i);
long long   get_time(void);
void		print_message(t_philo *philo, char *msg);
void		*philosopher(void *arg);
void    	clear_table(t_data *data);
int			check_dead(t_philo *philo, long long last_meal);
int			sleep_or_die(t_philo *philo, int sleep, long long last_meal);

#endif