#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include "libft.h"
# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdbool.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>

typedef enum e_booleen
{
	no = 0,
	yes = 1
}	t_booleen;

typedef struct s_forks
{
	int				*id;
	t_booleen		*is_used;
	pthread_mutex_t	id_mutex;
	pthread_mutex_t	is_used_mutex;
}	t_forks;

typedef struct s_data
{
	int				nb_fork;
	int				nb_philo;
	int				number_of_times_each_philosopher_must_eat;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				stop;
	long int		t_start;
	// pthread_mutex_t dead_mutex;
	// pthread_mutex_t write_mutex;
	// pthread_mutex_t print_mutex;
	// pthread_mutex_t eat_mutex;
	// pthread_mutex_t sleep_mutex;
	// pthread_mutex_t think_mutex;
	long long		first_timestamp;
	t_booleen		dead;
}	t_data;

typedef struct s_philo
{
	int				id;
	t_booleen		eat;
	t_booleen		sleep;
	t_booleen		think;
	int				is_using_forks;
	long long		t_last_meal;
	t_forks			*forks;
	t_data			*data;
}	t_philo;


#endif