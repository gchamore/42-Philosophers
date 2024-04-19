/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchamore <gchamore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 12:13:13 by gchamore          #+#    #+#             */
/*   Updated: 2024/04/19 16:03:10 by gchamore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include "libft.h"
# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/time.h>
# include <limits.h>

typedef enum e_booleen
{
	no = 0,
	yes = 1
}	t_booleen;

typedef struct s_philo
{
	t_booleen		*dead;
	t_booleen		dead_flag;
	t_booleen		eating;
	int				id;
	int				nb_meals_eat;
	int				num_of_philos;
	int				num_times_to_eat;
	pthread_mutex_t	dead_mutex_flag;
	pthread_mutex_t	*dead_mutex;
	pthread_mutex_t	*finish_eat_mutex;
	pthread_mutex_t	*print_mutex;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_t		*thread;
	long long		last_meal;
	long long		start;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
}					t_philo;

// MAIN

int			main(int ac, char **av);
void		ft_end(t_philo *philo, pthread_mutex_t *forks);

// PARSING

void		ft_parsing_1(int ac, char **av);
void		ft_parsing_2(char **av);

// INIT

void		ft_init_argv(t_philo *philo, char **argv);
void		ft_init_philos(t_philo *philo, pthread_mutex_t *forks, char **argv);
void		ft_thread_creator(t_philo *philo, \
pthread_t *brain_thread);
void		ft_thread_joined(t_philo *philo, pthread_t *brain_thread);


// PHILO'S LIFE

void		*philo_routine(void *arg);
int			ft_even(t_philo *philo);
int			ft_odd(t_philo *philo);
void		ft_eat(t_philo *philo);
int			someone_is_dead(t_philo *philo);

// BRAIN LIFE

void		*brain_routine(void *arg);
int			brain_dead_checker(t_philo *philo);
int			philosopher_dead(t_philo *philo, long long time_to_die);
int			brain_finish_eating_checker(t_philo *philo);

// UTILS

void		print_message(char *str, t_philo *philo, int id);
long long	time_diff(long long present, long long past);
long long	ft_timestamp(void);
void		ft_usleep(long long milliseconds);
void		ft_error(char *str, t_philo *philo, pthread_mutex_t *forks);

#endif