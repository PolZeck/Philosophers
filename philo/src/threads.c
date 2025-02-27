/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 10:29:36 by pledieu           #+#    #+#             */
/*   Updated: 2025/02/27 16:09:24 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

/**
 * @brief Starts the philosopher simulation.
 * 
 * This function:
 * - Creates threads for each philosopher.
 * - Launches a monitoring thread to check if a 
 *   philosopher dies (`monitor_death`).
 * - If `num_meals` is set, launches a thread to 
 *   monitor the meals (`monitor_meals`).
 * - Waits for all philosopher threads to finish.
 * - Ensures proper cleanup by joining all threads.
 * 
 * @param data Pointer to `t_data` storing all simulation data.
 * 
 * @return Returns `0` on success, `1` if thread creation fails.
 * 
 * @note If thread creation fails, an error message is printed, 
 *       and the function exits with `1`.
 * @note `pthread_join()` ensures that all threads are properly 
 *       cleaned up before exiting.
 */

int	start_simulation(t_data *data)
{
	pthread_t	death_checker;
	pthread_t	meals_checker;
	int			i;

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
	i = -1;
	while (++i < data->num_philos)
	{
		if (pthread_join(data->philos[i].thread, NULL))
		{
			printf("❌ Erreur en rejoignant le thread du philosophe %d\n", i);
			return (1);
		}
	}
	pthread_join(death_checker, NULL);
	return (0);
}
/**
 * @brief Monitors the philosophers to detect if one dies.
 * 
 * This function:
 * - Continuously checks if the simulation is still running.
 * - Iterates through all philosophers to verify if one has died.
 * - If a philosopher dies, it stops the simulation.
 * - Uses a mutex (`death_lock`) to ensure safe access to 
 *   `simulation_running`.
 * 
 * @param arg Pointer to `t_data` structure containing all 
 *            simulation data.
 * 
 * @return Always returns `NULL` when the monitoring loop stops.
 * 
 * @note If `simulation_running` is set to `0`, the function exits.
 * @note The function runs in a separate thread created by 
 *       `pthread_create()`.
 * @note Uses `usleep(500)` to reduce CPU usage.
 */

void	*monitor_death(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		pthread_mutex_lock(&data->death_lock);
		if (!data->simulation_running)
		{
			pthread_mutex_unlock(&data->death_lock);
			return (NULL);
		}
		pthread_mutex_unlock(&data->death_lock);
		i = 0;
		while (i < data->num_philos)
		{
			if (death_check(data, i))
				return (NULL);
			i++;
		}
		usleep(500);
	}
	return (NULL);
}
/**
 * @brief Monitors meal consumption to stop the simulation 
 *        when all philosophers have eaten enough.
 * 
 * This function:
 * - Continuously checks if the simulation is still running.
 * - Counts the number of philosophers who have reached 
 *   the required number of meals.
 * - If all philosophers have eaten `num_meals` times, 
 *   it stops the simulation.
 * - Uses a mutex (`death_lock`) to safely update 
 *   `simulation_running`.
 * 
 * @param arg Pointer to `t_data` structure containing all 
 *            simulation data.
 * 
 * @return Always returns `NULL` when the monitoring loop stops.
 * 
 * @note If all philosophers have eaten `num_meals` times, 
 *       a message is printed and the simulation stops.
 * @note The function runs in a separate thread created 
 *       by `pthread_create()`.
 * @note Uses `usleep(1000)` to reduce CPU usage.
 */

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
/**
 * @brief Defines the main routine for each philosopher.
 * 
 * This function:
 * - Delays execution for even-numbered philosophers 
 *   to prevent deadlocks.
 * - Runs an infinite loop where the philosopher:
 *   - Checks if the simulation is still running.
 *   - Eats, then sleeps and thinks.
 * - Uses a mutex (`death_lock`) to safely check 
 *   `simulation_running`.
 * - Exits when `simulation_running` is set to `0`.
 * 
 * @param arg Pointer to `t_philo` structure representing 
 *            the philosopher.
 * 
 * @return Always returns `NULL` when the philosopher exits.
 * 
 * @note Even-numbered philosophers sleep briefly (`usleep(1000)`) 
 *       at the start to reduce contention.
 * @note The function runs in a separate thread created 
 *       by `pthread_create()`.
 */

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		pthread_mutex_lock(&philo->data->death_lock);
		if (!philo->data->simulation_running)
		{
			pthread_mutex_unlock(&philo->data->death_lock);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->data->death_lock);
		eat(philo);
		if (!philo->data->simulation_running)
			return (NULL);
		sleep_and_think(philo);
	}
}
/**
 * @brief Checks if a philosopher has exceeded `time_to_die` 
 *        and stops the simulation if necessary.
 * 
 * This function:
 * - Computes the time since the last meal of a philosopher.
 * - If this time exceeds `time_to_die`, it sets 
 *   `simulation_running` to `0` and prints a death message.
 * - Uses `death_lock` to safely check and update 
 *   `simulation_running`.
 * - Uses `write_lock` to ensure safe output printing.
 * 
 * @param data Pointer to `t_data`, which stores all simulation data.
 * @param i Index of the philosopher to check.
 * 
 * @return Returns `1` if the philosopher has died, `0` otherwise.
 * 
 * @note If a philosopher dies, a message is printed, and 
 *       the simulation stops.
 * @note The function ensures thread safety with mutexes 
 *       (`death_lock` and `write_lock`).
 */

int	death_check(t_data *data, int i)
{
	long long	time_since_meal;

	pthread_mutex_lock(&data->death_lock);
	time_since_meal = get_timestamp() - data->philos[i].last_meal;
	if (time_since_meal > data->time_to_die && data->simulation_running)
	{
		data->simulation_running = 0;
		pthread_mutex_unlock(&data->death_lock);
		pthread_mutex_lock(&data->write_lock);
		printf("%lld %d died\n",
			get_timestamp() - data->start_time, data->philos[i].id);
		pthread_mutex_unlock(&data->write_lock);
		return (1);
	}
	pthread_mutex_unlock(&data->death_lock);
	return (0);
}
