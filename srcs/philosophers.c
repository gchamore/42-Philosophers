/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchamore <gchamore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 10:19:18 by gchamore          #+#    #+#             */
/*   Updated: 2024/04/10 17:01:45 by gchamore         ###   ########.fr       */
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

long long	time_diff(long long past, long long pres)
{
	return (pres - past);
}

long long	timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_free_forks(t_forks *forks, t_data *data)
{
	int i;

	i = 0;
	while (i < data->nb_philo)
	{
		free(forks[i].id);
		free(forks[i].is_used);
    }
	free(forks);
}

void	ft_usleep(int ms)
{
	long int	time;

	time = timestamp();
	while (timestamp() - time < ms)
		usleep(ms / 10);
}

// ROUTINE PHILOSOPHERS

void *philo_routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;
	int	id_philo = philo->id;
	printf("PHILO %d\n", id_philo + 1);
	if (philo->data->dead == no)
	{
		// pthread_mutex_lock(&philo->data->write_mutex);
		// printf("PHILO = %d\n forks = %d \n nb_fork = %d\n\n", philo->id + 1, philo->forks, philo->data->nb_fork);
		// pthread_mutex_unlock(&philo->data->write_mutex);
		if (philo->eat == yes && philo->is_using_forks == 2)
		{
			printf("PHILO %d is eating\n", philo->id + 1);
			// usleep(philo->data->time_to_eat);
			// philo->last_meal_time = timestamp();
			// pthread_mutex_lock(&philo->forks->is_used_mutex);
			if (philo->id % 2 == 0)
			{
				if ( philo->id == 0)
				{
					*(philo->forks[philo->id].is_used) = no;
					*(philo->forks[philo->data->nb_philo - 1].is_used) = no;
				}
				else
				{
					*(philo->forks[philo->id].is_used) = no;
					*(philo->forks[philo->id - 1].is_used) = no;
				}
			}
			else
			{
				*(philo->forks[philo->id].is_used) = no;
				*(philo->forks[philo->id - 1].is_used) = no;
			}
			// pthread_mutex_unlock(&philo->forks->is_used_mutex);
			philo->is_using_forks = 0;
			philo->eat = no;
			philo->sleep = yes;
			philo->data->number_of_times_each_philosopher_must_eat--;
			if (philo->sleep == yes && philo->is_using_forks == 0)
			{
				printf("PHILO %d is sleeping\n", id_philo + 1);
				// usleep(philo->data->time_to_sleep);
				philo->sleep = no;
				philo->think = yes;
			}
		}
		else if (philo->think == yes && philo->is_using_forks == 0)
		{
			printf("PHILO %d is thinking\n", id_philo + 1);
			// pthread_mutex_lock(&philo->forks->is_used_mutex);
			if (id_philo == 0)
			{
				if ((philo->forks[id_philo].is_used == no) && (philo->forks[philo->data->nb_philo - 1].is_used == no))
				{
					philo[id_philo].is_using_forks = 2;
					philo[id_philo].eat = yes;
					philo[id_philo].sleep = no;
					philo[id_philo].think = no;
					*(philo->forks[id_philo].is_used) = yes;
					*(philo->forks[philo->data->nb_philo - 1].is_used) = yes;
				}
			}
			else if (id_philo > 0)
			{
				if ((philo->forks[id_philo].is_used == no) && (philo->forks[id_philo - 1].is_used == no))
				{
					philo[id_philo].is_using_forks = 2;
					philo[id_philo].eat = yes;
					philo[id_philo].sleep = no;
					philo[id_philo].think = no;
					*(philo->forks[id_philo].is_used) = yes;
					*(philo->forks[id_philo - 1].is_used) = yes;
				}
			}
			// pthread_mutex_unlock(&philo->forks->is_used_mutex);
			// if (time_diff(philo->last_meal_time, timestamp()) > philo->data->time_to_die)
			// {
			// 	printf("PHILO %d is dead\n", philo->id + 1);
			// 	philo->data->dead = yes;
			// 	return (0);
			// }
		}
		printf("HEY\n");
	}
	return (0);
}

void	ft_init_forks(t_forks *forks, t_data *data)
{
	int i;

	i = 0;
	while (i < data->nb_philo)
	{
		forks[i].id = malloc(sizeof(int));
		forks[i].is_used = malloc(sizeof(t_booleen));
		*(forks[i].id) = i;
		*(forks[i].is_used) = no;
		i++;
    }
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
// void	ft_check_id(t_philo *philo, t_data *data)
// {
// 	int i;

// 	i = 0;
// 	while (i < data->nb_philo)
// 	{
// 		;
// 		i++;
// 	}
// }

// void	ft_check_if_used(t_forks *forks, t_data *data)
// {
// 	int i;

// 	i = 0;
// 	while (i < data->nb_philo)
// 	{
// 		printf("forks[%d].is_used = %d\n", i, forks[i].is_used);
// 		i++;
// 	}
// }

void	ft_init_philo(t_philo *philo, t_data *data)
{
	int id_philo;

	id_philo = 0;
	while(id_philo < data->nb_philo)
	{
		if(id_philo % 2 == 0)
		{
			if (id_philo == 0)
			{
				if ((*(philo->forks[id_philo].is_used) == no) && (*(philo->forks[data->nb_philo - 1].is_used) == no))
				{
					philo[id_philo].is_using_forks = 2;
					philo[id_philo].eat = yes;
					philo[id_philo].sleep = no;
					philo[id_philo].think = no;
					*(philo->forks[id_philo].is_used) = yes;
					*(philo->forks[data->nb_philo - 1].is_used) = yes;
				}
			}
			else
			{
				if((*(philo->forks[id_philo].is_used) == no) && (*(philo->forks[id_philo - 1].is_used) == no))
				{
					philo[id_philo].is_using_forks = 2;
					philo[id_philo].eat = yes;
					philo[id_philo].sleep = no;
					philo[id_philo].think = no;
					*(philo->forks[id_philo].is_used) = yes;
					*(philo->forks[id_philo - 1].is_used) = yes;
				}
				else
				{
					philo[id_philo].is_using_forks = 0;
					philo[id_philo].eat = no;
					philo[id_philo].sleep = no;
					philo[id_philo].think = yes;
				}
			}
		}
		else
		{
			philo[id_philo].is_using_forks = 0;
			philo[id_philo].eat = no;
			philo[id_philo].sleep = no;
			philo[id_philo].think = yes;
		}
		philo[id_philo].id = id_philo;
		id_philo++;
	}
}


// gettimeofday(&current_time, NULL);
//   	printf("seconds : %ld\nmicro seconds : %ld",
//     	current_time.tv_sec, current_time.tv_usec);
//MAIN

void	ft_print_philo(t_philo *philo, t_data *data)
{
	int i;

	i = 0;
	while (i < data->nb_philo)
	{
		printf("------------PHILO %d-------------\n", i + 1);
		if (philo[i].eat == 1)
			printf("philo %d is eating \n", i + 1);
		if (philo[i].sleep == 1)
			printf("philo %d is sleeping \n", i + 1);
		if (philo[i].think == 1)
			printf("philo %d is thinking \n", i + 1);
		if (philo[i].is_using_forks == 2)
			printf("he can eat he has 2 forks\n");
		else
			printf("he can't eat he has 0 forks\n");
		if (*(philo->forks[i].is_used) == 1)
			printf("philo's fork is USED\n");
		else if (*(philo->forks[i].is_used) == 0)
			printf("philo's fork is NOT USED\n");
		printf("\n");
		i++;
	}
}

int main(int ac, char **av)
{
	int i;
    t_philo	*philo;
	t_data	data;
	t_forks	*forks;

	i = 0;
    ft_parsing_1(ac, av);
	data = ft_init_data(&data, av);
	forks = malloc(data.nb_philo * sizeof(t_forks));
	ft_init_forks(forks, &data);
	philo = malloc(data.nb_philo * sizeof(t_philo));
	pthread_t *philo_threads = malloc(data.nb_philo * sizeof(pthread_t));
    if (!philo || !philo_threads)
    {
        perror("ERROR: memory allocation failed");
        exit(EXIT_FAILURE);
    }
	philo->forks = forks;
	ft_init_philo(philo, &data);
	printf("------------------------INIT------------------------\n\n");
	ft_print_philo(philo, &data);
	printf("----------------------------------------------------\n\n");

    // Création des états des philosophes
    // Initialisation du mutex
    pthread_mutex_init(&philo->forks->id_mutex, NULL);
	pthread_mutex_init(&philo->forks->is_used_mutex, NULL);
	// pthread_mutex_init(&data.write_mutex, NULL);
	// pthread_mutex_init(&data.eat_mutex, NULL);
	// pthread_mutex_init(&data.sleep_mutex, NULL);
	// pthread_mutex_init(&data.think_mutex, NULL);

	// pthread_mutex_lock(&philo->data->dead_mutex);
    // Création des threads pour les philosophes
	i = 0;
	while (i < data.nb_philo)
	{
		philo[i].data = &data;
		if (pthread_create(&philo_threads[i], NULL, philo_routine, &philo[i]) != 0)
		{
			perror("Thread creation error");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	// usleep(10000);
	i = 0;
	printf("111111 ----------- TEST\n\n");
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
	printf("OOOOOO ----------- TEST\n\n");
    // Affichage des états des philosophes
    pthread_mutex_destroy(&philo->forks->id_mutex);
	pthread_mutex_destroy(&philo->forks->is_used_mutex);
	// pthread_mutex_destroy(&data.eat_mutex);
	// pthread_mutex_destroy(&data.sleep_mutex);
	// pthread_mutex_destroy(&data.think_mutex);
    // Libération de mémoire
	ft_free_forks(forks, &data);
    free(philo_threads);
	free(philo);
	printf("22222222 ----------- TEST\n\n");
    return 0;
}