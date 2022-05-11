/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alistair <alistair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 17:04:29 by alkane            #+#    #+#             */
/*   Updated: 2022/05/11 02:39:27 by alistair         ###   ########.fr       */
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

struct	s_data;

typedef struct s_philo
{
	int				id;
	int				n_eaten;
	int				left_fork;
	int				right_fork;
	long long		last_meal;
	struct s_data	*data;
	pthread_t		thread_id;
}					t_philo;

typedef struct s_data
{
	int				n_philo;
	int				tt_die;
	int				tt_eat;
	int				tt_sleep;
	int				all_eaten;
	int				n_meal;
	int				dead_flag;
	long long		start_ts;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	fork_array[250];
	t_philo			philos[250];
}				t_data;

long		ft_atoi(const char *nptr);
int			set_table(t_data *data, int argc, char **argv);
long long	get_time(void);
void		print_message(t_philo *philo, char *msg);
void		clear_table(t_data *data);
void		do_stuff(t_data *data, long long stuff_time);
void		check_dead(t_philo philo);
void		eating(t_philo *philo);

#endif