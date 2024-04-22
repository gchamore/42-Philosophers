/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchamore <gchamore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 10:19:18 by gchamore          #+#    #+#             */
/*   Updated: 2024/04/22 14:49:43 by gchamore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

// // MAIN FUNCTION

int	main(int ac, char **av)
{
	t_philo			*philo;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
	pthread_t		brain_thread;

	ft_parsing_1(ac, av);
	philo = malloc(sizeof(t_philo) * ft_atoi(av[1]));
	if (!philo)
		return (0);
	forks = malloc(sizeof(pthread_mutex_t) * ft_atoi(av[1]));
	if (!forks)
		return (free(philo), 0);
	philo->dead_flag = no;
	ft_init_forks(forks, av);
	pthread_mutex_init(&philo->dead_mutex_flag, NULL);
	pthread_mutex_init(&print, NULL);
	if (ft_init_philos(philo, forks, av, &print) == 1)
		return (1);
	if (ft_thread_creator(philo, &brain_thread) == 1)
		return (ft_stop(philo, forks), 1);
	if (ft_thread_joined(philo, &brain_thread) == 1)
		return (ft_stop(philo, forks), 1);
	ft_stop(philo, forks);
	return (0);
}

// END (FREE AND DESTROY)

void	ft_stop(t_philo *philo, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	if (philo->print_mutex)
		pthread_mutex_destroy(philo->print_mutex);
	pthread_mutex_destroy(&philo->dead_mutex_flag);
	while (i < philo->num_of_philos)
	{
		if (philo[i].finish_eat_mutex)
		{
			pthread_mutex_destroy(philo[i].finish_eat_mutex);
			free(philo[i].finish_eat_mutex);
			philo[i].finish_eat_mutex = NULL;
		}
		if (&forks[i])
			pthread_mutex_destroy(&forks[i]);
		i++;
	}
	if (philo)
		free(philo);
	if (forks)
		free(forks);
}
