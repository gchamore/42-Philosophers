/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchamore <gchamore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 12:54:18 by gchamore          #+#    #+#             */
/*   Updated: 2024/04/22 15:10:15 by gchamore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

// INIT INPUTS

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

// INIT FORKS

void	ft_init_forks(pthread_mutex_t *forks, char **argv)
{
	int	i;

	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
}	

// INIT PHILOS

int	ft_init_philos(t_philo *philo, pthread_mutex_t *forks, char **argv, \
pthread_mutex_t *print)
{
	int	i;

	i = -1;
	while (++i < ft_atoi(argv[1]))
	{
		philo[i].id = i + 1;
		philo[i].eating = no;
		philo[i].nb_meals_eat = 0;
		ft_init_argv(&philo[i], argv);
		philo[i].start = ft_timestamp();
		philo[i].last_meal = ft_timestamp();
		philo[i].finish_eat_mutex = malloc(sizeof(pthread_mutex_t));
		if (!philo[i].finish_eat_mutex)
			return (ft_stop(philo, forks), 1);
		philo[i].dead_mutex = &philo->dead_mutex_flag;
		philo[i].print_mutex = print;
		pthread_mutex_init(philo[i].finish_eat_mutex, NULL);
		philo[i].dead = &philo->dead_flag;
		philo[i].left_fork = &forks[i];
		if (i == 0)
			philo[i].right_fork = &forks[philo[i].num_of_philos - 1];
		else
			philo[i].right_fork = &forks[i - 1];
	}
	return (0);
}

// THREADS CREATION (PHILO AND BRAIN)

int	ft_thread_creator(t_philo *philo, pthread_t *brain_thread)
{
	int	i;

	i = 0;
	if (pthread_create(brain_thread, NULL, &brain_routine, philo) != 0)
		return (1);
	while (i < philo->num_of_philos)
	{
		pthread_create(&philo[i].thread, NULL, &philo_routine, &philo[i]);
		i++;
	}
	return (0);
}

// JOIN THREADS (PHILO AND BRAIN)

int	ft_thread_joined(t_philo *philo, pthread_t *brain_thread)
{
	int	i;

	i = 0;
	if (pthread_join(*brain_thread, NULL) != 0)
		return (1);
	while (i < philo->num_of_philos)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	return (0);
}
