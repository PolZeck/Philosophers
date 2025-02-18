/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_philo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 10:30:09 by pledieu           #+#    #+#             */
/*   Updated: 2025/02/18 11:05:47 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/philosophers.h"

int	check_death(t_data *data)
{
	int	i;

	while (data->simulation_running)
	{
		i = 0;
		while (i < data->num_philos)
		{
			pthread_mutex_lock(&data->death_lock);
			if ((get_timestamp() - data->philos[i].last_meal) > data->time_to_die)
			{
				data->simulation_running = 0;
				print_status(&data->philos[i], "died");
				pthread_mutex_unlock(&data->death_lock);
				return (1);
			}
			pthread_mutex_unlock(&data->death_lock);
			i++;
		}
		usleep(1000);
	}
	return (0);
}
