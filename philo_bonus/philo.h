/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 17:04:29 by alkane            #+#    #+#             */
/*   Updated: 2022/05/14 12:02:58 by alkane           ###   ########.fr       */
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
# include <sys/wait.h>

# include <fcntl.h>
# include <sys/stat.h>
# include <semaphore.h>

# define SEMAFORK "/semaphore_forks"
# define SEMA_PRINT "/semaphore_print"
# define SEMA_THREAD "/semaphore_thread"
# define SEMA_DEATH "/semaphore_death"
# define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)

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
	
	sem_t			*print_lock;
	sem_t			*thread_lock;
	sem_t			*dead_lock;
	
	sem_t			*semaforks;
	t_philo			philos[250];
}				t_data;	

long		ft_atoi(const char *nptr);
int			set_table(t_data *data, int argc, char **argv);
long long	get_time(void);
void		print_message(t_philo *philo, char *msg);
void		clear_table(t_data *data);
void		spend_time(t_data *data, long long stuff_time);

int			chk_philo_death(t_philo philo);
int			chk_dead(t_data *data);
int			chk_ph_meals(t_philo philo);
int			chk_total_eat(t_data *data);

sem_t		*open_take(char *sem_name);
void		release_sema(sem_t	*semaphore);

#endif