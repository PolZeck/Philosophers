/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_parse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 10:29:19 by pledieu           #+#    #+#             */
/*   Updated: 2025/02/27 16:03:13 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

static int	parse_arguments(t_data *data, char **av)
{
	data->num_meals = -1;
	data->num_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (av[5])
	{
		data->num_meals = ft_atoi(av[5]);
		if (data->num_meals <= 0)
			return (printf("Invalid arguments\n"), 1);
	}
	if (data->num_philos <= 0 || data->time_to_die <= 0
		|| data->time_to_eat <= 0 || data->time_to_sleep <= 0)
		return (printf("Invalid arguments\n"), 1);
	return (0);
}
/**
 * @brief Initializes the `t_data` structure for the simulation.
 * 
 * This function:
 * - Resets all fields of `t_data` using `memset()`.
 * - Parses user arguments via `parse_arguments()`.
 * - Allocates memory for philosophers (`t_philo`) 
 *   and forks (`pthread_mutex_t`).
 * - Initializes mutexes used for thread synchronization.
 * 
 * @param data Pointer to `t_data`, storing simulation data.
 * @param av Array of strings containing program arguments.
 * 
 * @return Returns `0` if successful, `1` if an error occurs.
 * 
 * @note If memory allocation fails, an error is printed 
 *       and the function returns `1`.
 * @note If `parse_arguments()` fails, the function exits 
 *       immediately with `1`.
 * @note All mutexes (forks, `write_lock`, `death_lock`) 
 *       are initialized for thread safety.
 */

int	init_data(t_data *data, char **av)
{
	int	i;

	memset(data, 0, sizeof(t_data));
	if (parse_arguments(data, av))
		return (1);
	data->simulation_running = 1;
	data->start_time = get_timestamp();
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->forks || !data->philos)
		return (printf("Memory allocation failed\n"), 1);
	i = -1;
	while (++i < data->num_philos)
		pthread_mutex_init(&data->forks[i], NULL);
	pthread_mutex_init(&data->write_lock, NULL);
	pthread_mutex_init(&data->death_lock, NULL);
	return (0);
}

int	init_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		memset(&data->philos[i], 0, sizeof(t_philo));
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal = data->start_time;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		data->philos[i].data = data;
		i++;
	}
	return (0);
}
