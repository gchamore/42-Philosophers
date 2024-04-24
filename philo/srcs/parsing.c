/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchamore <gchamore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 12:54:39 by gchamore          #+#    #+#             */
/*   Updated: 2024/04/24 15:14:18 by gchamore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosophers.h"

// PARSING

void	ft_parsing_2(char **av)
{
	if (ft_atoi(av[1]) == 0 || ft_atoi(av[2]) == 0 || \
	ft_atoi(av[3]) == 0 || ft_atoi(av[4]) == 0)
	{
		printf("ERROR one arg is = to 0 you need to put a valid number\n");
		exit(0);
	}
	if (ft_atoi(av[1]) > 200)
	{
		printf("Nop on a dit pas plus de 200 philosophers !\n");
		exit(0);
	}
}

void	ft_parsing_1(int ac, char **av)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	if (ac != 5 && ac != 6)
	{
		printf("ERROR not good amount of arg %d instead of 4 or 5\n", ac - 1);
		exit(0);
	}
	while (av[i] && (ac - 1) != 0)
	{
		j = -1;
		while (av[i][++j])
		{
			if (av[i][j] < 48 || av[i][j] > 57 || j > 5)
			{
				printf("ERROR in arg %s, not valid digit or too long\n", av[i]);
				exit(0);
			}
		}
		i++;
		ac--;
	}
	ft_parsing_2(av);
}
