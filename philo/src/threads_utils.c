/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:28:31 by pledieu           #+#    #+#             */
/*   Updated: 2025/02/27 16:36:52 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

/**
 * @brief Handles the case where a philosopher is alone 
 *        and takes a fork.
 * 
 * This function:
 * - Makes the philosopher take a single fork.
 * - Prints the action using `print_status()`.
 * - Waits for `time_to_die` milliseconds, simulating 
 *   the philosopher starving to death.
 * - Releases the fork mutex before returning.
 * 
 * @param philo Pointer to the `t_philo` structure representing 
 *              the philosopher.
 * 
 * @note This function is only used when `num_philos == 1`, 
 *       as a single philosopher cannot eat.
 * @note The philosopher always dies after `time_to_die` 
 *       milliseconds because they cannot pick up a second fork.
 */

void	take_fork_alone(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	ft_usleep(philo->data->time_to_die, philo->data);
	pthread_mutex_unlock(philo->left_fork);
}
/**
 * @brief Checks if the simulation is still running.
 * 
 * This function:
 * - Locks the `death_lock` mutex to safely check the 
 *   `simulation_running` status.
 * - Returns `0` if the simulation has stopped.
 * - Returns `1` if the simulation is still active.
 * - Ensures proper unlocking of `death_lock` before returning.
 * 
 * @param philo Pointer to the `t_philo` structure representing 
 *              the philosopher.
 * 
 * @return Returns `1` if the simulation is running, `0` otherwise.
 * 
 * @note The function ensures thread-safe access to 
 *       `simulation_running` using a mutex.
 * @note If `simulation_running == 0`, the function exits 
 *       early to prevent unnecessary execution.
 */

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
/**
 * @brief Creates a thread for each philosopher.
 * 
 * This function:
 * - Iterates through all philosophers and creates a thread 
 *   for each one.
 * - Calls `philosopher_routine()` as the entry point 
 *   for each philosopher thread.
 * - Uses `usleep(100)` between thread creations to prevent 
 *   race conditions at startup.
 * - Returns `1` and prints an error message if thread 
 *   creation fails.
 * 
 * @param data Pointer to the `t_data` structure containing 
 *             all simulation data.
 * 
 * @return Returns `0` on success, `1` if a thread creation 
 *         fails.
 * 
 * @note The function ensures smooth initialization by adding 
 *       a slight delay (`usleep(100)`) between thread spawns.
 * @note If a thread creation fails, the function immediately 
 *       returns and prints an error message.
 */

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
/**
 * @brief Counts the number of philosophers who have eaten 
 *        the required number of meals.
 * 
 * This function:
 * - Iterates through all philosophers to check their 
 *   `meals_eaten` count.
 * - Uses a mutex (`death_lock`) to safely read 
 *   `meals_eaten` and `num_meals`.
 * - Increments `finished_meals` for each philosopher 
 *   who has eaten at least `num_meals` times.
 * 
 * @param data Pointer to the `t_data` structure containing 
 *             all simulation data.
 * 
 * @return The number of philosophers who have finished 
 *         eating the required meals.
 * 
 * @note Uses `death_lock` to ensure thread safety when 
 *       accessing `meals_eaten`.
 * @note This function is used in `monitor_meals()` to 
 *       determine when the simulation should stop.
 */

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
