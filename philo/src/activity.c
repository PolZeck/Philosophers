/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activity.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 10:29:04 by pledieu           #+#    #+#             */
/*   Updated: 2025/02/27 10:35:43 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	assign_forks(t_philo *philo, pthread_mutex_t **first,
	pthread_mutex_t **second)
{
	if (philo->id % 2 == 0)
	{
		*first = philo->right_fork;
		*second = philo->left_fork;
	}
	else
	{
		*first = philo->left_fork;
		*second = philo->right_fork;
	}
}

int	check_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_lock);
	if (get_timestamp() - philo->last_meal > philo->data->time_to_die)
	{
		pthread_mutex_unlock(&philo->data->death_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->death_lock);
	return (0);
}

static void	one_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	ft_usleep(philo->data->time_to_die, philo->data);
	print_status(philo, "died");
	pthread_mutex_lock(&philo->data->death_lock);
	philo->data->simulation_running = 0;
	pthread_mutex_unlock(&philo->data->death_lock);
	pthread_mutex_unlock(philo->left_fork);
}

void	eat(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (philo->data->num_philos == 1)
	{
		one_philo(philo);
		return ;
	}
	assign_forks(philo, &first_fork, &second_fork);
	if (check_death(philo))
		return ;
	pthread_mutex_lock(first_fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(second_fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->death_lock);
	philo->last_meal = get_timestamp();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->death_lock);
	print_status(philo, "is eating");
	ft_usleep(philo->data->time_to_eat, philo->data);
	pthread_mutex_unlock(second_fork);
	pthread_mutex_unlock(first_fork);
}

void	sleep_and_think(t_philo *philo)
{
	print_status(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep, philo->data);
	pthread_mutex_lock(&philo->data->death_lock);
	if (!philo->data->simulation_running)
	{
		pthread_mutex_unlock(&philo->data->death_lock);
		return ;
	}
	pthread_mutex_unlock(&philo->data->death_lock);
	print_status(philo, "is thinking");
}
