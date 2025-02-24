/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 08:14:33 by pledieu           #+#    #+#             */
/*   Updated: 2025/02/24 11:11:05 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 5 && ac != 6)
	{
		printf("Usage: ./philo num_philos time_to_die");
		printf("time_to_eat time_to_sleep [num_meals]\n");
		return (1);
	}
	if (init_data(&data, av) || init_philosophers(&data))
	{
		cleanup(&data);
		return (1);
	}
	if (start_simulation(&data))
	{
		cleanup(&data);
		return (1);
	}
	cleanup(&data);
	return (0);
}
