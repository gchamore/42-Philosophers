/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchamore <gchamore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 10:19:18 by gchamore          #+#    #+#             */
/*   Updated: 2024/04/12 14:44:33 by gchamore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

// PARSING
void	ft_parsing_2(char **av)
{
	if (ft_atoi(av[1]) == 0 || ft_atoi(av[2]) == 0 || ft_atoi(av[3]) == 0 || ft_atoi(av[4]) == 0)
	{
		printf("ERROR one arg is = a 0 you need to put a valid number\n");
		exit(0);
	}
	if (ft_atoi(av[1]) > 200)
	{
		printf("ERROR there  is more than 200 philosophers\n");
		exit(0);
	}
}

void	ft_parsing_1(int ac, char **av)
{
	int i;
	int j;

	i = 1;
	j = 0;
	if (ac != 5 && ac != 6)
	{
		printf("ERROR not good amount of arg %d au lieu de 5 ou 6\n", ac);
		exit(0);
	}
	while(av[i] && (ac - 1) != 0)
	{
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] < 48 || av[i][j] > 57)
			{
				printf("ERROR in arg %s, it is not a digit\n", av[i]);
				exit(0);
			}
			j++;
		}
		i++;
		ac--;
	}
	ft_parsing_2(av);
}

int	someone_is_dead(t_philo *philo)
{
	pthread_mutex_lock(philo->dead_mutex);
	if (*philo->dead == yes)
		return (pthread_mutex_unlock(philo->dead_mutex), 1);
	pthread_mutex_unlock(philo->dead_mutex);
	return (0);
}

long long	time_diff(long long past, long long pres)
{
	return (pres - past);
}

void	print_message(char *str, t_philo *philo, int id)
{
	long long	time;

	time = time_diff(philo->start, ft_timestamp());
	pthread_mutex_lock(philo->print_mutex);
	if (someone_is_dead(philo) == no)
		printf("%lld %d %s\n", time, id, str);
	pthread_mutex_unlock(philo->print_mutex);
}
// INIT DATA

void	ft_init_argv(t_philo *philo, char **argv)
{
	philo->time_to_die = ft_atoi(argv[2]);
	philo->time_to_eat = ft_atoi(argv[3]);
	philo->time_to_sleep = ft_atoi(argv[4]);
	philo->num_of_philos = ft_atoi(argv[1]);
	if (argv[5])
		philo->num_times_to_eat = ft_atoi(argv[5]);
	else
		philo->num_times_to_eat = -1;
}

// INIT PHILOS

void	ft_init_philos(t_philo *philos, t_brain *brain, pthread_mutex_t *forks, char **argv)
{
	int	i;

	i = -1;
	while (++i < ft_atoi(argv[1]))
	{
		philos[i].id = i + 1;
		philos[i].eating = no;
		philos[i].nb_meals_eat = 0;
		ft_init_argv(&philos[i], argv);
		philos[i].start = ft_timestamp();
		philos[i].last_meal = ft_timestamp();
		// printf("philo[%d].last_meal = %lld\n", i, philos[i].last_meal);
		philos[i].print_mutex = malloc(sizeof(pthread_mutex_t));
        philos[i].dead_mutex = malloc(sizeof(pthread_mutex_t));
        philos[i].eat_mutex = malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(philos[i].print_mutex, NULL);
        pthread_mutex_init(philos[i].dead_mutex, NULL);
        pthread_mutex_init(philos[i].eat_mutex, NULL);
		philos[i].dead = &brain->dead_flag;
		philos[i].left_fork = &forks[i];
		if (i == 0)
			philos[i].right_fork = &forks[philos[i].num_of_philos - 1];
		else
			philos[i].right_fork = &forks[i - 1];
	}
}

// Philo Threads routine

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		ft_usleep(50);
	while (someone_is_dead(philo) == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_message("has taken a fork", philo, philo->id);
		if (philo->num_of_philos == 1)
		{
			ft_usleep(philo->time_to_die);
			pthread_mutex_unlock(philo->right_fork);
		}
		pthread_mutex_lock(philo->left_fork);
		print_message("has taken a fork", philo, philo->id);
		philo->eating = yes;
		print_message("is eating", philo, philo->id);
		pthread_mutex_lock(philo->eat_mutex);
		philo->last_meal = ft_timestamp();
		philo->nb_meals_eat++;
		pthread_mutex_unlock(philo->eat_mutex);
		// printf("philo[%d].last_meal = %lld\n", philo->id, philo->last_meal);
		// printf("philo[%d].nb_meals_eat = %d\n", philo->id, philo->nb_meals_eat);
		ft_usleep(philo->time_to_eat);
		philo->eating = no;
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		print_message("is sleeping", philo, philo->id);
		ft_usleep(philo->time_to_sleep);
		print_message("is thinking", philo, philo->id);
		// if (
		// 	philo->num_times_to_eat != -1
		// 	&& philo->nb_meals_eat >= philo->num_times_to_eat
		// )
		// 	break ;
		// )
	}
	return (arg);
}

// CHECK IF DEAD

int	philosopher_dead(t_philo *philo, long long time_to_die)
{
	pthread_mutex_lock(philo->eat_mutex);
	if ((ft_timestamp() - philo->last_meal) >= time_to_die && philo->eating == no)
	{
		pthread_mutex_unlock(philo->eat_mutex);
		return (1);
	}
	pthread_mutex_unlock(philo->eat_mutex);
	return (0);
}

// Monitor Threads routine

void	*monitor_routine(void *arg)
{
	t_philo	*philo;
	int	i;
	int	finished_eating;

	philo = (t_philo *)arg;
	while (1)
	{
		if (philo->num_times_to_eat == -1)
			break;
		i = 0;
		while (i < philo->num_of_philos)
		{
			if (philosopher_dead(&philo[i], philo[i].time_to_die) == 1)
			{
				print_message("died", &philo[i], philo[i].id);
				pthread_mutex_lock(philo->dead_mutex);
				*philo->dead = yes;
				pthread_mutex_unlock(philo->dead_mutex);
				break;
			}
			i++;
		}
		i = 0;
		finished_eating = 0;
		while (i < philo->num_of_philos)
		{
			pthread_mutex_lock(philo[i].eat_mutex);
			if (philo[i].nb_meals_eat >= philo[i].num_times_to_eat)
				finished_eating++;
			pthread_mutex_unlock(philo[i].eat_mutex);
			i++;
		}
		if (finished_eating == philo->num_of_philos)
		{
			pthread_mutex_lock(philo->dead_mutex);
			*philo->dead = yes;
			pthread_mutex_unlock(philo->dead_mutex);
			break;
		}
	}
	return (arg);
}

int main(int ac, char **av)
{
	t_brain			brain;
	t_philo			*philo;
	pthread_mutex_t	*forks;
	pthread_t	manager_thread;
	int	i;
	
	ft_parsing_1(ac, av);
	philo = malloc(sizeof(t_philo) * ft_atoi(av[1]));
	forks = malloc(sizeof(pthread_mutex_t) * ft_atoi(av[1]));
	brain.dead_flag = no;
	brain.philo = philo;
	i = 0;
	while (i < ft_atoi(av[1]))
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	ft_init_philos(philo, &brain, forks, av);

	if (pthread_create(&manager_thread, NULL, &monitor_routine, brain.philo) != 0)
		ft_clear("Thread creation error", &brain, forks);
	i = 0;
	while (i < brain.philo->num_of_philos)
	{
		if (pthread_create(&brain.philo[i].thread, NULL, &philo_routine,
				&brain.philo[i]) != 0)
			ft_clear("Thread creation error", &brain, forks);
		i++;
	}
	i = 0;
	if (pthread_join(manager_thread, NULL) != 0)
		ft_clear("Thread join error", &brain, forks);
	while (i < brain.philo->num_of_philos)
	{
		if (pthread_join(brain.philo[i].thread, NULL) != 0)
			ft_clear("Thread join error", &brain, forks);
		i++;
	}
	i = 0;
	while (i < brain.philo->num_of_philos)
	{
        pthread_mutex_destroy(philo[i].print_mutex);
        pthread_mutex_destroy(philo[i].dead_mutex);
        pthread_mutex_destroy(philo[i].eat_mutex);
        free(philo[i].print_mutex);
        free(philo[i].dead_mutex);
        free(philo[i].eat_mutex);
        i++;
    }
	free(philo);
	free(forks);
	return (0);
}