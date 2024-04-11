/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchamore <gchamore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 13:40:02 by gchamore          #+#    #+#             */
/*   Updated: 2024/04/11 14:10:25 by gchamore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

// int	ft_strlen(char *str)
// {
// 	int	i;

// 	if (str == NULL)
// 		return (0);
// 	i = 0;
// 	while (str[i] != '\0')
// 		i++;
// 	return (i);
// }
// // Own version of atoi

// int	ft_atoi(char *str)
// {
// 	unsigned long long	nb;
// 	int					sign;
// 	int					i;

// 	nb = 0;
// 	sign = 1;
// 	i = 0;
// 	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
// 		|| str[i] == '\f' || str[i] == '\r')
// 		i++;
// 	if (str[i] == '-')
// 		sign = -1;
// 	if (str[i] == '-' || str[i] == '+')
// 		i++;
// 	while (str[i] >= '0' && str[i] <= '9')
// 	{
// 		nb = nb * 10 + (str[i] - '0');
// 		i++;
// 	}
// 	return (sign * nb);
// }
// Destroys all the mutexes

void	destory_all(char *str, t_brain *brain, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	if (str)
	{
		write(2, str, ft_strlen(str));
		write(2, "\n", 1);
	}
	pthread_mutex_destroy(&brain->print_mutex);
	pthread_mutex_destroy(&brain->eat_mutex);
	pthread_mutex_destroy(&brain->dead_mutex);
	while (i < brain->philo[0].num_of_philos)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
}

// Improved version of sleep function

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = ft_timestamp();
	while ((ft_timestamp() - start) < milliseconds)
		usleep(500);
	return (0);
}

// Gets the current time in milliseconds

size_t	ft_timestamp(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}