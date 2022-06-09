/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 17:04:29 by alkane            #+#    #+#             */
/*   Updated: 2022/06/09 12:35:52 by alkane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h> 
# include <stdint.h>
# include <sys/time.h>
# include <time.h>
# include <unistd.h>
# include <stdlib.h>

struct	s_data;

typedef struct s_philo
{
	int				id;
	int				n_eaten;
	long long		last_meal;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	checking;
	pthread_t		thread_id;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	int				n_philo;
	int				tt_die;
	int				tt_eat;
	int				tt_sleep;
	int				n_meal;
	int				philos_done_eating;
	int				done_flag;
	long long		start_t;
	t_philo			philos[250];
	pthread_mutex_t	fork_array[250];
	pthread_mutex_t	done_lock;
}				t_data;

// libft
long		ft_atoi(const char *nptr);

// init.c
int			set_table(t_data *data, int argc, char **argv);

// philo.c
void		*ph_func(void *arg);

// utils.c
void		spend_time(long long time_ms);
long long	get_time(void);
void		print_message(t_philo *philo, char *msg);

// waiters.c
void		*waiter_thread(void *arg);
void		*finish_eating_waiter(void *arg);
int			check_done_flag(t_data *data);

#endif