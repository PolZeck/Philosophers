/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 10:29:36 by pledieu           #+#    #+#             */
/*   Updated: 2025/02/19 15:07:48 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	*philosopher_routine(void *arg)
{
	t_philo	*philo = (t_philo *)arg;

	if (philo->data->num_philos == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		ft_usleep(philo->data->time_to_die);
		pthread_mutex_unlock(philo->left_fork);
		return (NULL);
	}

	while (1)
	{
		pthread_mutex_lock(&philo->data->death_lock);
		if (!philo->data->simulation_running)
		{
			pthread_mutex_unlock(&philo->data->death_lock);
			break;
		}
		pthread_mutex_unlock(&philo->data->death_lock);

		eat(philo);
		if (!philo->data->simulation_running) break;
		sleep_and_think(philo);
	}
	return (NULL);
}



int start_simulation(t_data *data)
{
    pthread_t death_checker;

    for (int i = 0; i < data->num_philos; i++)
    {
        if (pthread_create(&data->philos[i].thread, NULL, philosopher_routine, &data->philos[i]))
            return (printf("Thread creation failed\n"), 1);
        usleep(100); // Petit délai pour éviter la congestion
    }

    if (pthread_create(&death_checker, NULL, monitor_death, data))
        return (printf("Death monitor thread failed\n"), 1);

    for (int i = 0; i < data->num_philos; i++)
        pthread_join(data->philos[i].thread, NULL);
    pthread_join(death_checker, NULL);

    return (0);
}

void *monitor_death(void *arg)
{
    t_data *data = (t_data *)arg;
    int i;

    while (data->simulation_running)
    {
        i = 0;
        while (i < data->num_philos)
        {
            pthread_mutex_lock(&data->death_lock);
            long long time_since_meal = get_timestamp() - data->philos[i].last_meal;

            if (time_since_meal > data->time_to_die && data->simulation_running)
            {
                data->simulation_running = 0;
                pthread_mutex_unlock(&data->death_lock);

                pthread_mutex_lock(&data->write_lock);
                printf("%lld %d died\n", get_timestamp() - data->start_time, data->philos[i].id);
                pthread_mutex_unlock(&data->write_lock);

                return (NULL);
            }
            pthread_mutex_unlock(&data->death_lock);
            i++;
        }
        usleep(500);
    }
    return (NULL);
}


