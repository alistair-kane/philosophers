/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alistair <alistair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 17:04:29 by alkane            #+#    #+#             */
/*   Updated: 2022/06/02 07:59:57 by alistair         ###   ########.fr       */
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
	int				eating;
	int				n_eaten;
	int				left_fork;
	int				right_fork;
	long long		last_meal;
	long long		limit;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	int				n_philo;
	int				tt_die;
	int				tt_eat;
	int				tt_sleep;
	int				n_meal;
	int				done_flag;
	long long		start_ts;
	int				fork_states[250];
	t_philo			philos[250];
	pthread_mutex_t	fork_array[250];
	pthread_mutex_t	print_lock;
	pthread_mutex_t	done_lock;
	pthread_mutex_t	meal_lock;
}				t_data;

long		ft_atoi(const char *nptr);
int			set_table(t_data *data, int argc, char **argv);
long long	get_time(void);
void		print_message(t_philo *philo, char *msg, int action);
void		spend_time(long long time_ms);
int			chk_philo_death(t_philo *philo);
void		tidy_up(t_data *data);


#endif