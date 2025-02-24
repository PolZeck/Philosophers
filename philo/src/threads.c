/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 10:29:36 by pledieu           #+#    #+#             */
/*   Updated: 2025/02/24 15:43:50 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	start_simulation(t_data *data)
{
	pthread_t	death_checker;
	pthread_t	meals_checker;
	int			i;

	memset(&death_checker, 0, sizeof(pthread_t));
	memset(&meals_checker, 0, sizeof(pthread_t));
	if (create_philo_threads(data))
		return (1);
	if (pthread_create(&death_checker, NULL, monitor_death, data))
		return (printf("Death monitor thread failed\n"), 1);
	if (data->num_meals > 0)
	{
		if (pthread_create(&meals_checker, NULL, monitor_meals, data))
			return (printf("Meals monitor thread failed\n"), 1);
		pthread_join(meals_checker, NULL);
	}
	i = 0;
	while (i < data->num_philos)
		pthread_join(data->philos[i++].thread, NULL);
	pthread_join(death_checker, NULL);
	return (0);
}

void	*monitor_death(void *arg)
{
	t_data	*data;
	int		i;
	int		finished_meals;

	data = (t_data *)arg;
	while (data->simulation_running)
	{
		finished_meals = 0;
		i = 0;
		while (i < data->num_philos)
		{
			if (death_check(data, i))
				return (NULL);
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

void	*monitor_meals(void *arg)
{
	t_data	*data;
	int		finished_meals;

	data = (t_data *)arg;
	while (data->simulation_running)
	{
		finished_meals = count_finished_meals(data);
		if (finished_meals == data->num_philos)
		{
			printf("All philosophers have eaten at least %d meals. "
				"Ending simulation.\n", data->num_meals);
			pthread_mutex_lock(&data->death_lock);
			data->simulation_running = 0;
			pthread_mutex_unlock(&data->death_lock);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->num_philos == 1)
	{
		take_fork_alone(philo);
		return (NULL);
	}
	while (1)
	{
		if (!check_simulation_running(philo))
			break ;
		eat(philo);
		if (!check_simulation_running(philo))
			break ;
		sleep_and_think(philo);
	}
	return (NULL);
}

int	death_check(t_data *data, int i)
{
	long long	time_since_meal;

	pthread_mutex_lock(&data->death_lock);
	time_since_meal = get_timestamp() - data->philos[i].last_meal;
	if (time_since_meal > data->time_to_die && data->simulation_running)
	{
		handle_death(data, i);
		return (1);
	}
	pthread_mutex_unlock(&data->death_lock);
	return (0);
}
