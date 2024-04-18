/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchamore <gchamore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 12:54:05 by gchamore          #+#    #+#             */
/*   Updated: 2024/04/18 13:21:09 by gchamore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

// PHILO'S THREADS ROUTINE

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (someone_is_dead(philo) == 0)
	{
		if (philo->id % 2 == 0)
		{
			if (ft_even(philo) == 1)
				return (arg);
		}
		else if (philo->id % 2 == 1)
		{
			if (ft_odd(philo) == 1)
				return (arg);
		}
		ft_eat(philo);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		print_message("is sleeping", philo, philo->id);
		ft_usleep(philo->time_to_sleep);
		print_message("is thinking", philo, philo->id);
		usleep(50);
	}
	return (arg);
}

int	ft_even(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_message("has taken a fork", philo, philo->id);
	if (philo->num_of_philos == 1)
	{
		ft_usleep(philo->time_to_die);
		pthread_mutex_unlock(philo->left_fork);
		return (1);
	}
	pthread_mutex_lock(philo->right_fork);
	print_message("has taken a fork", philo, philo->id);
	return (0);
}

int	ft_odd(t_philo *philo)
{
	usleep(500);
	pthread_mutex_lock(philo->right_fork);
	print_message("has taken a fork", philo, philo->id);
	if (philo->num_of_philos == 1)
	{
		ft_usleep(philo->time_to_die);
		pthread_mutex_unlock(philo->right_fork);
		return (1);
	}
	pthread_mutex_lock(philo->left_fork);
	print_message("has taken a fork", philo, philo->id);
	return (0);
}

void	ft_eat(t_philo *philo)
{
	philo->eating = yes;
	print_message("is eating", philo, philo->id);
	pthread_mutex_lock(philo->finish_eat_mutex);
	philo->last_meal = ft_timestamp();
	philo->nb_meals_eat++;
	pthread_mutex_unlock(philo->finish_eat_mutex);
	ft_usleep(philo->time_to_eat);
	philo->eating = no;
}

// CHECK IF SOMEONE IS DEAD

int	someone_is_dead(t_philo *philo)
{
	pthread_mutex_lock(philo->dead_mutex);
	if (*philo->dead == yes)
	{
		pthread_mutex_unlock(philo->dead_mutex);
		return (1);
	}
	pthread_mutex_unlock(philo->dead_mutex);
	return (0);
}
