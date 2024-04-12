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

typedef struct s_philo
{
	t_booleen		*dead;
	t_booleen		eating;
	int				id;
	int				nb_meals_eat;
	int				num_of_philos;
	int				num_times_to_eat;
	pthread_mutex_t	*dead_mutex;
	pthread_mutex_t	*eat_mutex;
	pthread_mutex_t	*print_mutex;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_t		thread;
	long long		last_meal;
	long long		start;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
}					t_philo;

typedef struct s_brain
{
	t_booleen		dead_flag;
	t_philo			*philo;
}					t_brain;


// Utils
void		ft_clear(char *str, t_brain *brain, pthread_mutex_t *forks);
int			ft_usleep(long long milliseconds);
long long		ft_timestamp(void);


#endif