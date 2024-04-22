/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchamore <gchamore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 13:40:02 by gchamore          #+#    #+#             */
/*   Updated: 2024/04/22 10:37:31 by gchamore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

// PRINT A MESSAGE WITH CURRENT TIME + PHILO'S ID + MESSAGE

void	print_message(char *str, t_philo *philo, int id)
{
	long long	time;

	time = time_diff(ft_timestamp(), philo->start);
	pthread_mutex_lock(philo->print_mutex);
	if (someone_is_dead(philo) == no)
		printf("%lld %d %s\n", time, id, str);
	pthread_mutex_unlock(philo->print_mutex);
}

// RETURN TIME DIFFERENCE BETWEEN TWO TIMESTAMP

long long	time_diff(long long present, long long past)
{
	return (present - past);
}

// CURRENT TIMESTAMP IN MILLISECONDS

long long	ft_timestamp(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

// USLEEP FUNCTION IN MILLISECONDS

void	ft_usleep(long long milliseconds)
{
	milliseconds = milliseconds * 1000;
	usleep(milliseconds);
}

// // ERROR (DESTROY MUTEXES AND FREE MEM)

// void	ft_error(char *str, t_philo *philo, pthread_mutex_t *forks)
// {
// 	int	i;

// 	i = 0;
// 	if (str)
// 	{
// 		write(2, str, ft_strlen(str));
// 		write(2, "\n", 1);
// 	}
// 	pthread_mutex_destroy(&philo->dead_mutex_flag);
// 	while (i < philo->num_of_philos)
// 	{
// 		pthread_mutex_destroy(philo[i].print_mutex);
// 		pthread_mutex_destroy(philo[i].dead_mutex);
// 		pthread_mutex_destroy(philo[i].finish_eat_mutex);
// 		if (philo[i].print_mutex)
// 			free(philo[i].print_mutex);
// 		if (philo[i].finish_eat_mutex)
// 			free(philo[i].finish_eat_mutex);
// 		pthread_mutex_destroy(&forks[i]);
// 		if (philo[i].thread)
// 			free(philo[i].thread);
// 		i++;
// 	}
// 	if (philo)
// 		free(philo);
// 	if (forks)
// 		free(forks);
// 	exit(1);
// }
