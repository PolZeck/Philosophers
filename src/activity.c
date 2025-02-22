/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activity.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 10:29:04 by pledieu           #+#    #+#             */
/*   Updated: 2025/02/19 15:18:00 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	eat(t_philo *philo)
{
	pthread_mutex_t *first_fork;
	pthread_mutex_t *second_fork;

	if (philo->id % 2 == 0)
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	else
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}

	// ✅ Vérifier avant de prendre les fourchettes
	pthread_mutex_lock(&philo->data->death_lock);
	if (get_timestamp() - philo->last_meal > philo->data->time_to_die)
	{
		pthread_mutex_unlock(&philo->data->death_lock);
		return;
	}
	pthread_mutex_unlock(&philo->data->death_lock);

	pthread_mutex_lock(first_fork);
	print_status(philo, "has taken a fork");

	pthread_mutex_lock(second_fork);
	print_status(philo, "has taken a fork");

	pthread_mutex_lock(&philo->data->death_lock);
	philo->last_meal = get_timestamp();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->death_lock);

	print_status(philo, "is eating");
	ft_usleep(philo->data->time_to_eat);

	pthread_mutex_unlock(second_fork);
	pthread_mutex_unlock(first_fork);
}
void	sleep_and_think(t_philo *philo)
{
	print_status(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep);

	print_status(philo, "is thinking");
}
