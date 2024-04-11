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

# define PHILO_MAX 300

typedef enum e_booleen
{
	no = 0,
	yes = 1
}	t_booleen;

typedef struct s_philo
{
	int				*dead;
	int				eating;
	int				id;
	int				meals_eaten;
	int				num_of_philos;
	int				num_times_to_eat;
	pthread_mutex_t	*dead_mutex;
	pthread_mutex_t	*eat_mutex;
	pthread_mutex_t	*print_mutex;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_t		thread;
	size_t			last_meal;
	size_t			start_time;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
}					t_philo;
typedef struct s_brain
{
	int				dead_flag;
	pthread_mutex_t	dead_mutex;
	pthread_mutex_t	eat_mutex;
	pthread_mutex_t	print_mutex;
	t_philo			*philo;
}					t_brain;


// Utils
void		destory_all(char *str, t_brain *brain, pthread_mutex_t *forks);
int			ft_usleep(size_t milliseconds);
size_t		ft_timestamp(void);


#endif