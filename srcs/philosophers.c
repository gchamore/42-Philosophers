/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchamore <gchamore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 10:19:18 by gchamore          #+#    #+#             */
/*   Updated: 2024/04/08 18:17:01 by gchamore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

// PARSING
void	ft_parsing_2(char **av)
{
	if (ft_atoi(av[1]) == 0 || ft_atoi(av[2]) == 0 || ft_atoi(av[3]) == 0 || ft_atoi(av[4]) == 0)
	{
		printf("ERROR one arg is = a 0 you need to put a valid number\n");
		exit(0);
	}
	if (ft_atoi(av[1]) > 200)
	{
		printf("ERROR there  is more than 200 philosophers\n");
		exit(0);
	}
}

void	ft_parsing_1(int ac, char **av)
{
	int i;
	int j;

	i = 1;
	j = 0;
	if (ac != 5 && ac != 6)
	{
		printf("ERROR not good amount of arg %d au lieu de 5 ou 6\n", ac);
		exit(0);
	}
	while(av[i] && (ac - 1) != 0)
	{
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] < 48 || av[i][j] > 57)
			{
				printf("ERROR in arg %s, it is not a digit\n", av[i]);
				exit(0);
			}
			j++;
		}
		i++;
		ac--;
	}
	ft_parsing_2(av);
}


long long	timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_usleep(int ms)
{
	long int	time;

	time = timestamp();
	while (timestamp() - time < ms)
		usleep(ms / 10);
}
void	print(t_philo *philo, char *str)
{
	long int	time;

	pthread_mutex_lock(&(philo->data->print));
	time = timestamp() - philo->info->t_start;
	if (!philo->info->stop && time >= 0 \
			&& time <= INT_MAX && !is_dead(philo, 0))
		printf("%lld %d %s", timestamp() - philo->info->t_start, philo->n, str);
	pthread_mutex_unlock(&(philo->info->print));
}

// ROUTINE PHILOSOPHERS

void *philo_routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;
	// pthread_mutex_lock(&philo->data->write_mutex);
    printf("PHILO -> %d\n eat = %d\n forks = %d \n nb_fork = %d\n\n", philo->i + 1, philo->eat, philo->forks, philo->data->nb_fork);
	// pthread_mutex_unlock(&philo->data->write_mutex);
	if (philo->eat == yes && philo->forks == 2)
	{
		pthread_mutex_lock(&philo->data->eat_mutex);
		print(philo, " is eating\n");
		pthread_mutex_unlock(&philo->data->eat_mutex);
		usleep(philo->data->time_to_eat);
		philo->forks = 0;
		philo->data->nb_fork += 2;
		philo->eat = no;
		philo->sleep = yes;
		philo->data->number_of_times_each_philosopher_must_eat--;
		if (philo->sleep == yes && philo->forks == 0)
		{
			pthread_mutex_lock(&philo->data->sleep_mutex);
			printf("PHILO %d is sleeping\n", philo->i + 1);
			pthread_mutex_unlock(&philo->data->sleep_mutex);
			usleep(philo->data->time_to_sleep);
			philo->sleep = no;
			philo->think = yes;
		}
	}
	else if (philo->think == yes && philo->forks == 0)
	{
		pthread_mutex_lock(&philo->data->think_mutex);
		printf("PHILO %d is thinking\n", philo->i + 1);
		pthread_mutex_unlock(&philo->data->think_mutex);
		philo->eat = no;
		philo->sleep = no;
		philo->think = yes;
		if (philo->data->nb_fork >= 2)
		{
			philo->forks = 2;
			philo->data->nb_fork -= 2;
			philo->eat = yes;
			philo->sleep = no;
			philo->think = no;
		}
		else
		{
			pthread_mutex_lock(&philo->data->write_mutex);
			printf("PHILO %d is dead\n", philo->i + 1);
			pthread_mutex_unlock(&philo->data->write_mutex);
			philo->data->dead = yes;
			return (0);
		}
	}
	return 0;
}

// INIT DATA
t_data ft_init_data(t_data *data, char **av)
{
	data->nb_fork = atoi(av[1]);
	data->nb_philo = atoi(av[1]);
	data->time_to_die = atoi(av[2]);
	data->time_to_eat = atoi(av[3]);
	data->time_to_sleep = atoi(av[4]);
	data->number_of_times_each_philosopher_must_eat = atoi(av[5]);
	data->dead = no;
	return (*data);
}

t_data	ft_init_philo(t_philo *philo, t_data *data)
{
	int i;

	i = 0;
	
    while (i < data->nb_philo)
	{
		if(i % 2 == 0 && data->nb_fork > 1)
		{
			philo[i].forks = 2;
			philo[i].eat = yes;
			philo[i].sleep = no;
			philo[i].think = no;
			data->nb_fork -= 2;
		}
		else
		{
			philo[i].forks = 0;
			philo[i].eat = no;
			philo[i].sleep = no;
			philo[i].think = yes;
		}
		i++;
    }
	return (*data);
}

//MAIN
int main(int ac, char **av)
{
	int i;
    t_philo	*philo;
	t_data	data;
	struct timeval current_time;

	gettimeofday(&current_time, NULL);
  	printf("seconds : %ld\nmicro seconds : %ld",
    	current_time.tv_sec, current_time.tv_usec);
    ft_parsing_1(ac, av);
	data = ft_init_data(&data, av);
	philo = malloc(data.nb_philo * sizeof(t_philo));
	pthread_t *philo_threads = malloc(data.nb_philo * sizeof(pthread_t));
    if (!philo || !philo_threads)
    {
        perror("ERROR: memory allocation failed");
        exit(EXIT_FAILURE);
    }
	data = ft_init_philo(philo, &data);
		
    // Création des états des philosophes
    // Initialisation du mutex
    pthread_mutex_init(&data.dead_mutex, NULL);
	pthread_mutex_init(&data.write_mutex, NULL);
	pthread_mutex_init(&data.eat_mutex, NULL);
	pthread_mutex_init(&data.sleep_mutex, NULL);
	pthread_mutex_init(&data.think_mutex, NULL);

	// pthread_mutex_lock(&philo->data->dead_mutex);
	while (data.dead == no)
	{
    // Création des threads pour les philosophes
		i = 0;
		while (i < data.nb_philo)
		{
			philo[i].i = i;
			philo[i].data = &data;
			if (pthread_create(&philo_threads[i], NULL, philo_routine, &philo[i]) != 0)
			{
				perror("Thread creation error");
				exit(EXIT_FAILURE);
			}
			i++;
		}
		usleep(10000);
		i = 0;
		// Attente de la fin des threads
		while (i < data.nb_philo)
		{
			if (pthread_join(philo_threads[i], NULL) != 0)
			{
				perror("Thread join error");
				exit(EXIT_FAILURE);
			}
			i++;
		}
	}
	// pthread_mutex_unlock(&philo->data->dead_mutex);
    // Affichage des états des philosophes
    pthread_mutex_destroy(&data.dead_mutex);
	pthread_mutex_destroy(&data.write_mutex);
	pthread_mutex_destroy(&data.eat_mutex);
	pthread_mutex_destroy(&data.sleep_mutex);
	pthread_mutex_destroy(&data.think_mutex);
    // Libération de mémoire
    free(philo_threads);
	free(philo);
    return 0;
}