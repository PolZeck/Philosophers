/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activity.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 10:29:04 by pledieu           #+#    #+#             */
/*   Updated: 2025/02/18 12:45:27 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/philosophers.h"

void	eat(t_philo *philo)
{
	pthread_mutex_t *first_fork;
	pthread_mutex_t *second_fork;

	// ✅ Déterminer quelle fourchette prendre en premier
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

	pthread_mutex_lock(first_fork);
	print_status(philo, "has taken a fork");

	pthread_mutex_lock(second_fork);
	print_status(philo, "has taken a fork");

	// ✅ Mise à jour immédiate de last_meal
	pthread_mutex_lock(&philo->data->death_lock);
	philo->last_meal = get_timestamp();
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
