/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_philo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 10:30:09 by pledieu           #+#    #+#             */
/*   Updated: 2025/02/18 13:10:09 by pledieu          ###   ########lyon.fr   */
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
			long long current_time = get_timestamp();
			long long time_since_meal = current_time - data->philos[i].last_meal;
			pthread_mutex_unlock(&data->death_lock);

			// 🔥 DEBUG : Voir les temps de chaque philosophe
			printf("[DEBUG] Vérif mort : Philo %d - Dernier repas : %lld ms, Temps depuis repas : %lld ms\n",
				data->philos[i].id, data->philos[i].last_meal - data->start_time, time_since_meal);

			if (time_since_meal > data->time_to_die)
			{
				if (data->simulation_running)
				{
					data->simulation_running = 0;
					print_status(&data->philos[i], "died");
				}
				return (1);
			}
			i++;
		}
		usleep(500);
	}
	return (0);
}




