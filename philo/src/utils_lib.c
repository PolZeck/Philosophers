/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lib.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 10:29:58 by pledieu           #+#    #+#             */
/*   Updated: 2025/02/27 16:29:51 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

/**
 * @brief Gets the current timestamp in milliseconds.
 * 
 * This function retrieves the current time using `gettimeofday()`
 * and converts it into milliseconds.
 * 
 * @return The current time in milliseconds as a `long long`.
 * 
 * @note The function multiplies `tv_sec` by 1000 to convert 
 *       seconds into milliseconds.
 * @note It divides `tv_usec` by 1000 to convert microseconds 
 *       into milliseconds.
 */

long long	get_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
/**
 * @brief Sleeps for a given number of milliseconds while 
 *        checking if the simulation is still running.
 * 
 * This function:
 * - Records the current timestamp as the start time.
 * - Continuously checks if the elapsed time is less than 
 *   `time_in_ms`.
 * - Calls `usleep(100)` in small intervals to reduce CPU usage.
 * - Checks `simulation_running` using a mutex (`death_lock`) 
 *   to exit early if the simulation stops.
 * 
 * @param time_in_ms The duration to sleep in milliseconds.
 * @param data Pointer to `t_data`, used to check if the 
 *             simulation is still running.
 * 
 * @note Uses `usleep(100)` instead of a full sleep to allow 
 *       periodic checking of `simulation_running`.
 * @note If `simulation_running` is set to `0`, the function 
 *       exits immediately.
 */

void	ft_usleep(int time_in_ms, t_data *data)
{
	long long	start_time;

	start_time = get_timestamp();
	while ((get_timestamp() - start_time) < time_in_ms)
	{
		usleep(100);
		pthread_mutex_lock(&data->death_lock);
		if (!data->simulation_running)
		{
			pthread_mutex_unlock(&data->death_lock);
			break ;
		}
		pthread_mutex_unlock(&data->death_lock);
	}
}
/**
 * @brief Safely prints a message with a timestamp for a philosopher.
 * 
 * This function:
 * - Locks the `write_lock` mutex to ensure only one thread 
 *   prints at a time.
 * - Retrieves the current timestamp relative to the 
 *   simulation's start time.
 * - Prints the formatted message with the timestamp, 
 *   philosopher ID, and status message.
 * - Unlocks the `write_lock` mutex after printing.
 * 
 * @param philo Pointer to the `t_philo` structure representing 
 *              the philosopher.
 * @param msg The message to print (e.g., "is eating", 
 *            "has taken a fork").
 * 
 * @note Ensures thread-safe printing by using `write_lock`.
 * @note Prevents output corruption when multiple threads 
 *       print simultaneously.
 */

void	safe_print(t_philo *philo, const char *msg)
{
	long long	timestamp;

	pthread_mutex_lock(&philo->data->write_lock);
	timestamp = get_timestamp() - philo->data->start_time;
	printf("%lld %d %s\n", timestamp, philo->id, msg);
	pthread_mutex_unlock(&philo->data->write_lock);
}

void	print_status(t_philo *philo, const char *status)
{
	pthread_mutex_lock(&philo->data->death_lock);
	if (philo->data->simulation_running)
		safe_print(philo, status);
	pthread_mutex_unlock(&philo->data->death_lock);
}
