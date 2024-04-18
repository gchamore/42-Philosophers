/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchamore <gchamore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 10:19:18 by gchamore          #+#    #+#             */
/*   Updated: 2024/04/18 13:09:46 by gchamore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

// MAIN FUNCTION

int	main(int ac, char **av)
{
	t_philo			*philo;
	pthread_mutex_t	*forks;
	pthread_t		brain_thread;
	int				i;

	ft_parsing_1(ac, av);
	philo = malloc(sizeof(t_philo) * ft_atoi(av[1]));
	forks = malloc(sizeof(pthread_mutex_t) * ft_atoi(av[1]));
	if (!philo || !forks)
		ft_error("Malloc error", philo, forks);
	philo->dead_flag = no;
	i = 0;
	while (i < ft_atoi(av[1]))
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&philo->dead_mutex_flag, NULL);
	ft_init_philos(philo, forks, av);
	ft_thread_creator(philo, &brain_thread, forks);
	pthread_join(brain_thread, NULL);
	ft_end(philo, forks);
	return (0);
}
