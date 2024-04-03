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


typedef struct	s_mutex
{
	pthread_mutex_t mutex;
}	t_mutex;

typedef struct s_philo
{
	t_booleen		*eat;
	t_booleen		*sleep;
	t_booleen		*think;
	t_booleen		*dead;
	t_booleen		*fork;
}	t_philo;

typedef struct s_data
{
	int		*i;
	int		*nb_philo;
	int		*nb_fork;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	int		number_of_times_each_philosopher_must_eat;
	t_philo	*philo;
	t_mutex	*mutex;
}	t_data;

#endif