/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 10:29:36 by pledieu           #+#    #+#             */
/*   Updated: 2025/02/20 15:19:32 by pledieu          ###   ########lyon.fr   */
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
    pthread_t death_checker, meals_checker;

    for (int i = 0; i < data->num_philos; i++)
    {
        if (pthread_create(&data->philos[i].thread, NULL, philosopher_routine, &data->philos[i]))
            return (printf("Thread creation failed\n"), 1);
        usleep(100);
    }

    if (pthread_create(&death_checker, NULL, monitor_death, data))
        return (printf("Death monitor thread failed\n"), 1);

    if (data->num_meals > 0)
    {
        if (pthread_create(&meals_checker, NULL, monitor_meals, data))
            return (printf("Meals monitor thread failed\n"), 1);
        pthread_join(meals_checker, NULL);
    }

    for (int i = 0; i < data->num_philos; i++)
        pthread_join(data->philos[i].thread, NULL);
    
    pthread_join(death_checker, NULL);

    return (0);
}


void *monitor_death(void *arg)
{
    t_data *data = (t_data *)arg;
    int i, finished_meals;;

    while (data->simulation_running)
    {
		finished_meals = 0;
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
		if (finished_meals == data->num_philos)
        {
            pthread_mutex_lock(&data->death_lock);
            data->simulation_running = 0;
            pthread_mutex_unlock(&data->death_lock);
            return (NULL);
        }
        usleep(500);
    }
    return (NULL);
}

void *monitor_meals(void *arg)
{
    t_data *data = (t_data *)arg;
    int i, finished_meals;

    while (data->simulation_running)
    {
        finished_meals = 0;
        i = 0;
        while (i < data->num_philos)
        {
            pthread_mutex_lock(&data->death_lock);
            if (data->philos[i].meals_eaten >= data->num_meals && data->num_meals > 0)
                finished_meals++;
            pthread_mutex_unlock(&data->death_lock);
            i++;
        }
        if (finished_meals == data->num_philos)
        {
			printf("All philosophers have eaten at least %d meals. Ending simulation.\n", data->num_meals);
            pthread_mutex_lock(&data->death_lock);
            data->simulation_running = 0;
            pthread_mutex_unlock(&data->death_lock);
            return (NULL);
        }
        usleep(1000);
    }
    return (NULL);
}


