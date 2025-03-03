/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activity.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 10:29:04 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/03 09:37:11 by pledieu          ###   ########lyon.fr   */
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
/**
 * @brief Handles the case when there is only one philosopher.
 * 
 * This function:
 * - Makes the philosopher take a fork (since there is only one).
 * - Prints the action using `print_status()`.
 * - Waits for `time_to_die` milliseconds, simulating starvation.
 * - Prints the death message since the philosopher cannot eat.
 * - Sets `simulation_running` to `0` to stop the simulation.
 * - Unlocks the fork mutex before exiting.
 * 
 * @param philo Pointer to the `t_philo` structure representing 
 *              the philosopher.
 * 
 * @note This function is only called when `num_philos == 1`.
 * @note Since the philosopher cannot eat, they always die 
 *       after `time_to_die` milliseconds.
 * @note Uses `death_lock` to safely update `simulation_running`.
 */

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
/**
 * @brief Handles the eating routine of a philosopher.
 * 
 * This function:
 * - Manages the case where there is only one philosopher, 
 *   calling `one_philo()`.
 * - Assigns the correct forks based on the philosopher's ID.
 * - Checks if the philosopher has already died before eating.
 * - Locks the necessary forks, updates the last meal time, 
 *   and increases the meal count.
 * - Ensures proper synchronization using mutexes to avoid 
 *   race conditions.
 * - Sleeps for `time_to_eat` milliseconds before releasing 
 *   the forks.
 * 
 * @param philo Pointer to the `t_philo` structure representing 
 *              the philosopher.
 * 
 * @note If there is only one philosopher, they will take a fork 
 *       and die due to starvation.
 * @note Uses `death_lock` to safely update `last_meal` and 
 *       `meals_eaten` counters.
 */

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
/**
 * @brief Handles the sleeping and thinking routine of a philosopher.
 * 
 * This function:
 * - Prints the sleeping status and makes the philosopher 
 *   sleep for `time_to_sleep` milliseconds.
 * - Checks if the simulation is still running before 
 *   proceeding to the thinking phase.
 * - Prints the thinking status if the simulation is active.
 * - Uses a mutex (`death_lock`) to safely check if the 
 *   simulation should continue.
 * 
 * @param philo Pointer to the `t_philo` structure representing 
 *              the philosopher.
 * 
 * @note If `simulation_running` is set to `0`, the philosopher 
 *       stops instead of thinking.
 * @note Uses `ft_usleep()` to ensure accurate sleeping 
 *       without excessive CPU usage.
 */

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
