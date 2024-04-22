/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchamore <gchamore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 13:40:02 by gchamore          #+#    #+#             */
/*   Updated: 2024/04/22 15:02:06 by gchamore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

// PRINT A MESSAGE WITH CURRENT TIME + PHILO'S ID + MESSAGE

void	print_message(char *message, t_philo *philo, int id)
{
	long long	time;

	time = time_diff(ft_timestamp(), philo->start);
	pthread_mutex_lock(philo->print_mutex);
	if (someone_is_dead(philo) == no)
		printf("%lld %d %s\n", time, id, message);
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

int	ft_atoi(const char *message)
{
	int	i;
	int	total;
	int	signe;

	total = 0;
	i = 0;
	signe = 1;
	while ((message[i] >= 9 && message[i] <= 13) || message[i] == 32)
		i++;
	if (message[i] == '-' || message[i] == '+')
	{
		if (message[i] == '-')
		{
			signe = signe * (-1);
		}
		i++;
	}
	while (message[i] >= '0' && message[i] <= '9')
	{
		total = total * 10 + (message[i] - '0');
		i++;
	}
	return (total * signe);
}