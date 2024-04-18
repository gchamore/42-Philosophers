/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchamore <gchamore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 12:54:18 by gchamore          #+#    #+#             */
/*   Updated: 2024/04/18 13:20:58 by gchamore         ###   ########.fr       */
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

// INIT PHILOS

void	ft_init_philos(t_philo *philo, pthread_mutex_t *forks, char **argv)
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
		philo[i].print_mutex = malloc(sizeof(pthread_mutex_t));
		philo[i].finish_eat_mutex = malloc(sizeof(pthread_mutex_t));
		philo[i].dead_mutex = &philo->dead_mutex_flag;
		if (!philo[i].print_mutex || !philo[i].finish_eat_mutex)
			ft_error("Malloc error", philo, forks);
		pthread_mutex_init(philo[i].print_mutex, NULL);
		pthread_mutex_init(philo[i].finish_eat_mutex, NULL);
		philo[i].dead = &philo->dead_flag;
		philo[i].left_fork = &forks[i];
		if (i == 0)
			philo[i].right_fork = &forks[philo[i].num_of_philos - 1];
		else
			philo[i].right_fork = &forks[i - 1];
	}
}

// THREADS CREATION (PHILO AND BRAIN)

void	ft_thread_creator(t_philo *philo, pthread_t *brain_thread, \
pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	while (i < philo->num_of_philos)
	{
		if (pthread_create(&philo[i].thread, NULL, &philo_routine, \
		&philo[i]) != 0)
			ft_error("Thread creation error", philo, forks);
		i++;
	}
	if (pthread_create(brain_thread, NULL, &brain_routine, philo) != 0)
		ft_error("Thread creation error", philo, forks);
}

// JOIN THREADS (PHILO AND BRAIN)

void	ft_thread_joined(t_philo *philo, pthread_t brain_thread, \
pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	if (pthread_join(brain_thread, NULL) != 0)
		ft_error("Thread join error", philo, forks);
	while (i < philo->num_of_philos)
	{
		if (pthread_join(philo[i].thread, NULL) != 0)
			ft_error("Thread join error", philo, forks);
		i++;
	}
}

// END (FREE AND DESTROY)

void	ft_end(t_philo *philo, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&philo->dead_mutex_flag);
	while (i < philo->num_of_philos)
	{
		pthread_mutex_destroy(philo[i].print_mutex);
		pthread_mutex_destroy(philo[i].finish_eat_mutex);
		pthread_mutex_destroy(&forks[i]);
		free(philo[i].print_mutex);
		free(philo[i].finish_eat_mutex);
		i++;
	}
	free(philo);
	free(forks);
}
