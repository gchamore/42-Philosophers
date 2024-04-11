/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchamore <gchamore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 10:19:18 by gchamore          #+#    #+#             */
/*   Updated: 2024/04/11 14:11:49 by gchamore         ###   ########.fr       */
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

int	ft_dead_loop(t_philo *philo)
{
	pthread_mutex_lock(philo->dead_mutex);
	if (*philo->dead == 1)
		return (pthread_mutex_unlock(philo->dead_mutex), 1);
	pthread_mutex_unlock(philo->dead_mutex);
	return (0);
}

void	print_message(char *str, t_philo *philo, int id)
{
	size_t	time;

	pthread_mutex_lock(philo->print_mutex);
	time = ft_timestamp() - philo->start_time;
	if (!ft_dead_loop(philo))
		printf("%zu %d %s\n", time, id, str);
	pthread_mutex_unlock(philo->print_mutex);
}

long long	time_diff(long long past, long long pres)
{
	return (pres - past);
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

void	ft_init_philos(t_philo *philos, t_brain *brain, pthread_mutex_t *forks, char **argv)
{
	int	i;

	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		philos[i].id = i + 1;
		philos[i].eating = 0;
		philos[i].meals_eaten = 0;
		ft_init_argv(&philos[i], argv);
		philos[i].start_time = ft_timestamp();
		philos[i].last_meal = ft_timestamp();
		philos[i].print_mutex = &brain->print_mutex;
		philos[i].dead_mutex = &brain->dead_mutex;
		philos[i].eat_mutex = &brain->eat_mutex;
		philos[i].dead = &brain->dead_flag;
		philos[i].left_fork = &forks[i];
		if (i == 0)
			philos[i].right_fork = &forks[philos[i].num_of_philos - 1];
		else
			philos[i].right_fork = &forks[i - 1];
		i++;
	}
}

// Checks if the philosopher is dead

int	philosopher_dead(t_philo *philo, size_t time_to_die)
{
	pthread_mutex_lock(philo->eat_mutex);
	if (ft_timestamp() - philo->last_meal >= time_to_die
		&& philo->eating == 0)
		return (pthread_mutex_unlock(philo->eat_mutex), 1);
	pthread_mutex_unlock(philo->eat_mutex);
	return (0);
}

// Philo Threads routine

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		ft_usleep(1);
	while (!ft_dead_loop(philo))
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
		philo->eating = 1;
		print_message("is eating", philo, philo->id);
		pthread_mutex_lock(philo->eat_mutex);
		philo->last_meal = ft_timestamp();
		philo->meals_eaten++;
		pthread_mutex_unlock(philo->eat_mutex);
		ft_usleep(philo->time_to_eat);
		philo->eating = 0;
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		print_message("is sleeping", philo, philo->id);
		ft_usleep(philo->time_to_sleep);
		print_message("is thinking", philo, philo->id);
	}
	return (arg);
}

// Monitor Threads routine

int	check_if_all_ate(t_philo *philo)
{
	int	i;
	int	finished_eating;

	i = 0;
	finished_eating = 0;
	if (philo[0].num_times_to_eat == -1)
		return (0);
	while (i < philo[0].num_of_philos)
	{
		pthread_mutex_lock(philo[i].eat_mutex);
		if (philo[i].meals_eaten >= philo[i].num_times_to_eat)
			finished_eating++;
		pthread_mutex_unlock(philo[i].eat_mutex);
		i++;
	}
	if (finished_eating == philo[0].num_of_philos)
	{
		pthread_mutex_lock(philo[0].dead_mutex);
		*philo->dead = 1;
		pthread_mutex_unlock(philo[0].dead_mutex);
		return (1);
	}
	return (0);
}

int	check_if_dead(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo[0].num_of_philos)
	{
		if (philosopher_dead(&philo[i], philo[i].time_to_die))
		{
			print_message("died", &philo[i], philo[i].id);
			pthread_mutex_lock(philo[0].dead_mutex);
			*philo->dead = 1;
			pthread_mutex_unlock(philo[0].dead_mutex);
			return (1);
		}
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
		if (check_if_dead(philo) == 1 || check_if_all_ate(philo) == 1)
			break ;
	return (arg);
}

int main(int ac, char **av)
{
	t_brain		brain;
	t_philo			*philo;
	pthread_mutex_t	*forks;
	pthread_t	manager_thread;
	int	i;
	
	philo = malloc(sizeof(t_philo) * ft_atoi(av[1]));
	forks = malloc(sizeof(pthread_mutex_t) * ft_atoi(av[1]));
	ft_parsing_1(ac, av);
	brain.dead_flag = 0;
	brain.philo = philo;
	pthread_mutex_init(&brain.print_mutex, NULL);
	pthread_mutex_init(&brain.dead_mutex, NULL);
	pthread_mutex_init(&brain.eat_mutex, NULL);
	i = 0;
	while (i < ft_atoi(av[1]))
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	ft_init_philos(philo, &brain, forks, av);

	if (pthread_create(&manager_thread, NULL, &monitor_routine, brain.philo) != 0)
		destory_all("Thread creation error", &brain, forks);
	i = 0;
	while (i < brain.philo[0].num_of_philos)
	{
		if (pthread_create(&brain.philo[i].thread, NULL, &philo_routine,
				&brain.philo[i]) != 0)
			destory_all("Thread creation error", &brain, forks);
		i++;
	}
	i = 0;
	if (pthread_join(manager_thread, NULL) != 0)
		destory_all("Thread join error", &brain, forks);
	while (i < brain.philo[0].num_of_philos)
	{
		if (pthread_join(brain.philo[i].thread, NULL) != 0)
			destory_all("Thread join error", &brain, forks);
		i++;
	}
	free(philo);
	free(forks);
	return (0);
}