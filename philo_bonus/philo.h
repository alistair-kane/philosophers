/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkane <alkane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 17:04:29 by alkane            #+#    #+#             */
/*   Updated: 2022/06/08 13:57:11 by alkane           ###   ########.fr       */
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
# include <signal.h>

# define SEMA_DONE "/semaphore_done"
# define SEMA_PRINT "/semaphore_print"
# define SEMAFORKS "/semaphore_forks"
# define SEMA_FINISHED "/semaphore_thread"

# define PHILO_PREFIX "ph_"

struct	s_data;

typedef struct s_philo
{
	int				id;
	int				n_eaten;
	long long		last_meal;
	char			*name;
	sem_t			*checking;
	pid_t			pid;
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
	long long		start_ts;
	t_philo			philos[250];
	sem_t			*done_lock;
	sem_t			*print_lock;
	sem_t			*forks;
	sem_t			*finished;
}					t_data;

// libft
long		ft_atoi(const char *nptr);
void		*ft_calloc(size_t nmemb, size_t size);
char		*ft_itoa(int n);
char		*ft_strjoin(char const *s1, char const *s2);
size_t		ft_strlen(const char *s);

int			set_table(t_data *data, int argc, char **argv);
long long	get_time(void);
void		print_message(t_philo *philo, char *msg);
void		spend_time(long long stuff_time);

void		*monitor_thread(void *arg);
void		*monitor_thread_eat(void *arg);
void		*monitor_finish(void *arg);
int			tidy_up(t_data *data);

#endif