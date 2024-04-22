/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchamore <gchamore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 10:19:18 by gchamore          #+#    #+#             */
/*   Updated: 2024/04/22 10:50:52 by gchamore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

// // MAIN FUNCTION

// int	main(int ac, char **av)
// {
// 	t_philo			*philo;
// 	pthread_mutex_t	*forks;
// 	pthread_t		brain_thread;
// 	int				i;

// 	ft_parsing_1(ac, av);
// 	philo = malloc(sizeof(t_philo) * ft_atoi(av[1]));
// 	forks = malloc(sizeof(pthread_mutex_t) * ft_atoi(av[1]));
// 	if (!philo)
// 		return (0);
// 	if (!forks)
// 		return (free(philo), 0);
// 	philo->dead_flag = no;
// 	i = 0;
// 	while (i < ft_atoi(av[1]))
// 	{
// 		pthread_mutex_init(&forks[i], NULL);
// 		i++;
// 	}
// 	pthread_mutex_init(&philo->dead_mutex_flag, NULL);
// 	ft_init_philos(philo, forks, av);
// 	i = 0;
// 	while (i < philo->num_of_philos)
// 	{
// 		if (pthread_create(&philo[i].thread, NULL, &philo_routine, \
// 		&philo[i]) != 0)
// 			ft_error("Thread creation error", philo, forks, 1);
// 		i++;
// 	}
// 	if (pthread_create(&brain_thread, NULL, &brain_routine, philo) != 0)
// 		ft_error("Thread creation error", philo, forks, 1);
// 	i = 0;
// 	if (pthread_join(brain_thread, NULL) != 0)
// 		ft_error("Thread join error", philo, forks, 1);
// 	while (i < philo->num_of_philos)
// 	{
// 		if (pthread_join(philo[i].thread, NULL) != 0)
// 			ft_error("Thread join error", philo, forks, 1);
// 		i++;
// 	}
// 	i = 0;
// 	pthread_mutex_destroy(&philo->dead_mutex_flag);
// 	while (i < philo->num_of_philos)
// 	{
// 		pthread_mutex_destroy(philo[i].print_mutex);
// 		pthread_mutex_destroy(philo[i].finish_eat_mutex);
// 		pthread_mutex_destroy(&forks[i]);
// 		free(philo[i].print_mutex);
// 		free(philo[i].finish_eat_mutex);
// 		i++;
// 	}
// 	free(philo);
// 	free(forks);
// 	return (0);
// }

int	main(int ac, char **av)
{
	t_philo			*philo;
	pthread_mutex_t	*forks;
	pthread_t		brain_thread;
	int				i;

	ft_parsing_1(ac, av);
	philo = malloc(sizeof(t_philo) * ft_atoi(av[1]));
	if (!philo)
		return (0);
	forks = malloc(sizeof(pthread_mutex_t) * ft_atoi(av[1]));
	if (!forks)
		return (free(philo), 0);
	philo->dead_flag = no;
	i = 0;
	while (i < ft_atoi(av[1]))
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&philo->dead_mutex_flag, NULL);
	ft_init_philos(philo, forks, av);
	ft_thread_creator(philo, &brain_thread);
	ft_thread_joined(philo, &brain_thread);
	ft_stop(philo, forks);
	return (0);
}

// END (FREE AND DESTROY)

void	ft_stop(t_philo *philo, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&philo->dead_mutex_flag);
	while (i < philo->num_of_philos)
	{
		if (philo[i].print_mutex)
		{
			pthread_mutex_destroy(philo[i].print_mutex);
			free(philo[i].print_mutex);
			philo[i].print_mutex = NULL;
		}
		if (philo[i].finish_eat_mutex)
		{
			pthread_mutex_destroy(philo[i].finish_eat_mutex);
            free(philo[i].finish_eat_mutex);
            philo[i].finish_eat_mutex = NULL;
		}
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	if (philo)
		free(philo);
	if (forks)
		free(forks);
}
