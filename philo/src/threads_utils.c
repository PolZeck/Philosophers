/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:28:31 by pledieu           #+#    #+#             */
/*   Updated: 2025/02/26 11:08:20 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	take_fork_alone(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	ft_usleep(philo->data->time_to_die, philo->data);
	pthread_mutex_unlock(philo->left_fork);
}

int	check_simulation_running(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_lock);
	if (!philo->data->simulation_running)
	{
		pthread_mutex_unlock(&philo->data->death_lock);
		return (0);
	}
	pthread_mutex_unlock(&philo->data->death_lock);
	return (1);
}

int	create_philo_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
				philosopher_routine, &data->philos[i]))
			return (printf("Thread creation failed\n"), 1);
		usleep(100);
		i++;
	}
	return (0);
}

void	handle_death(t_data *data, int i)
{
	data->simulation_running = 0;
	pthread_mutex_unlock(&data->death_lock);
	pthread_mutex_lock(&data->write_lock);
	printf("%lld %d died\n", get_timestamp() - data->start_time,
		data->philos[i].id);
	pthread_mutex_unlock(&data->write_lock);
}

int	count_finished_meals(t_data *data)
{
	int	i;
	int	finished_meals;

	finished_meals = 0;
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->death_lock);
		if (data->philos[i].meals_eaten >= data->num_meals
			&& data->num_meals > 0)
			finished_meals++;
		pthread_mutex_unlock(&data->death_lock);
		i++;
	}
	return (finished_meals);
}
