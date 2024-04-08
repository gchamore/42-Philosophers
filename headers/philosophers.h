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

typedef enum e_booleen
{
	no = 0,
	yes = 1
}	t_booleen;

typedef struct s_data
{
	int				nb_fork;
	int				nb_philo;
	int				number_of_times_each_philosopher_must_eat;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	pthread_mutex_t dead_mutex;
	pthread_mutex_t write_mutex;
	pthread_mutex_t eat_mutex;
	pthread_mutex_t sleep_mutex;
	pthread_mutex_t think_mutex;
	t_booleen		dead;
}	t_data;

typedef struct s_philo
{
	int				i;
	t_booleen		eat;
	t_booleen		sleep;
	t_booleen		think;
	int				forks;
	t_data			*data;
}	t_philo;


#endif