/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchamore <gchamore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 10:19:18 by gchamore          #+#    #+#             */
/*   Updated: 2024/04/03 14:55:49 by gchamore         ###   ########.fr       */
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

// MALLOC PHILOSOPHERS

void ft_create_philosopher_states(t_data *data, int nb_philo)
{
	int i;

	i = 0;
    data->philo = (t_philo *)malloc(nb_philo * sizeof(t_philo));
    data->mutex = malloc(sizeof(t_mutex));
	data->i = malloc(sizeof(int));
	data->nb_philo = malloc(sizeof(int));
	data->nb_fork = malloc(sizeof(int));
	if (!data->philo || !data->mutex || !data->i || !data->nb_philo || !data->nb_fork)
	{
        perror("ERROR: memory allocation failed");
        exit(EXIT_FAILURE);
    }
    while (i < nb_philo)
	{
        data->philo[i].eat = malloc(sizeof(t_booleen));
        data->philo[i].sleep = malloc(sizeof(t_booleen));
        data->philo[i].think = malloc(sizeof(t_booleen));
		data->philo[i].dead = malloc(sizeof(t_booleen));
		data->philo[i].fork = malloc(sizeof(t_booleen));
        if (!data->philo[i].eat || !data->philo[i].sleep || !data->philo[i].think || !data->philo[i].dead || !data->philo[i].fork)
		{
            perror("ERROR: memory allocation failed");
            exit(EXIT_FAILURE);
        }
		*data->philo[i].fork = yes;
		i++;
    }
}

// void	ft_give_forks(t_philo *philo)
// {
// 	// printf("%d et %d\n", philo->data->i, philo->data->nb_fork);
// }

// ROUTINE PHILOSOPHERS

void *philo_routine(void *arg)
{
	t_data *data = arg;
	pthread_mutex_lock(&data->mutex->mutex);
	// ft_give_forks(philo);
	// *data->philo[*data->i].fork = no;
	printf("i = %d\n",*data->i);
	pthread_mutex_unlock(&data->mutex->mutex);
	return 0;
}

//MAIN

int main(int ac, char **av)
{
    // Validation des arguments
    ft_parsing_1(ac, av);
    // Allocation de mémoire pour les philosophes et les threads
    t_data *data = malloc(sizeof(t_data));
	pthread_t *philo_threads = malloc(atoi(av[1]) * sizeof(pthread_t));
    if (!data || !philo_threads)
    {
        perror("ERROR: memory allocation failed");
        exit(EXIT_FAILURE);
    }
    // Création des états des philosophes
    ft_create_philosopher_states(data, atoi(av[1]));
	*data->nb_philo = atoi(av[1]);
	*data->nb_fork = atoi(av[1]);
    // Initialisation des mutex
    pthread_mutex_init(&data->mutex->mutex, NULL);
    // Création des threads pour les philosophes
	printf("\n");
	*data->i = 0;
	// printf("*init data i nb 2 = %d\n",*data->i);
    while (*data->i < *data->nb_philo)
    {
		sleep(1);
        if (pthread_create(&philo_threads[*data->i], NULL, philo_routine, data) != 0)
        {
            perror("Thread creation error");
            exit(EXIT_FAILURE);
        }
		// printf("incrementation data i nb 2 = %d\n",*data->i);
		*data->i += 1;
    }
    // printf("philo = %d, eat = %d, sleep = %d, think = %d \n", *data->i, *data->philo[*data->i].eat, *data->philo[*data->i].sleep, *data->philo[*data->i].think);
    // Attente de la fin des threads
	printf("\n");
	*data->i = 0;
	// printf("*init data i nb 3 = %d\n",*data->i);
    while (*data->i < *data->nb_philo)
    {
        if (pthread_join(philo_threads[*data->i], NULL) != 0)
        {
            perror("Thread join error");
            exit(EXIT_FAILURE);
        }
		// printf("incrementation data i nb 3 = %d\n",*data->i);
		printf("fork = %d,\n",*(data->philo[*data->i].fork));

		*data->i += 1;
    }
	printf("\n");
    // Affichage des états des philosophes
    pthread_mutex_destroy(&data->mutex->mutex);
	*data->i = 0;
	// printf("*init data i nb 4 = %d\n",*data->i);
    while (*data->i < *data->nb_philo)
    {
	printf("fork = %d,\n",*(data->philo[*data->i].fork));
	    free(data->philo[*data->i].eat);
        free(data->philo[*data->i].sleep);
        free(data->philo[*data->i].think);
		free(data->philo[*data->i].dead);
		free(data->philo[*data->i].fork);
		// printf("incrementation data i nb 4 = %d\n",*data->i);
		*data->i += 1;
    }
    // Libération de mémoire
	free(data->i);
	free(data->nb_philo);
	free(data->nb_fork);
    free(data);
    free(philo_threads);
	free(data->philo);
    free(data->mutex);
    return 0;
}