/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brain_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchamore <gchamore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 12:54:14 by gchamore          #+#    #+#             */
/*   Updated: 2024/04/18 15:08:39 by gchamore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

// BRAIN THREAD ROUTINE

void	*brain_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (brain_dead_checker(philo) == 1)
			break ;
		if (philo->num_times_to_eat == -1)
			continue ;
		if (brain_finish_eating_checker(philo) == 1)
			break ;
	}
	return (arg);
}

// CHECK IF DEAD

int	brain_dead_checker(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->num_of_philos)
	{
		if (philosopher_dead(&philo[i], philo[i].time_to_die) == 1)
		{
			print_message("died", &philo[i], philo[i].id);
			pthread_mutex_lock(philo->dead_mutex);
			*philo->dead = yes;
			pthread_mutex_unlock(philo->dead_mutex);
			return (1);
		}
		i++;
	}
	return (0);
}

int	philosopher_dead(t_philo *philo, long long time_to_die)
{
	pthread_mutex_lock(philo->finish_eat_mutex);
	if ((ft_timestamp() - philo->last_meal) >= time_to_die \
	&& philo->eating == no)
	{
		pthread_mutex_unlock(philo->finish_eat_mutex);
		return (1);
	}
	pthread_mutex_unlock(philo->finish_eat_mutex);
	return (0);
}

// CHECK IF FINISHED EATING

int	brain_finish_eating_checker(t_philo *philo)
{
	int		i;
	int		finished_eating;

	i = 0;
	finished_eating = 0;
	while (i < philo->num_of_philos)
	{
		pthread_mutex_lock(philo[i].finish_eat_mutex);
		if (philo[i].nb_meals_eat == philo[i].num_times_to_eat)
			finished_eating++;
		pthread_mutex_unlock(philo[i].finish_eat_mutex);
		i++;
	}
	if (finished_eating == philo->num_of_philos)
	{
		pthread_mutex_lock(philo->dead_mutex);
		*philo->dead = yes;
		pthread_mutex_unlock(philo->dead_mutex);
		return (1);
	}
	return (0);
}
