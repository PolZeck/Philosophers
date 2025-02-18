/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_parse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 10:29:19 by pledieu           #+#    #+#             */
/*   Updated: 2025/02/18 20:58:07 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
		sign = (str[i++] == '-') ? -1 : 1;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i++] - '0');
		if (result > 2147483647)
			return (-1);
	}
	return ((int)(result * sign));
}

int	init_data(t_data *data, char **av)
{
	data->num_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->num_meals = (av[5]) ? ft_atoi(av[5]) : -1;
	if (data->num_philos <= 0 || data->time_to_die <= 0 || data->time_to_eat <= 0 || data->time_to_sleep <= 0)
		return (printf("Invalid arguments\n"), 1);
	data->simulation_running = 1;
	data->start_time = get_timestamp();
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->forks || !data->philos)
		return (printf("Memory allocation failed\n"), 1);
	for (int i = 0; i < data->num_philos; i++)
		pthread_mutex_init(&data->forks[i], NULL);
	pthread_mutex_init(&data->write_lock, NULL);
	pthread_mutex_init(&data->death_lock, NULL);
	return (0);
}

int	init_philosophers(t_data *data)
{
	for (int i = 0; i < data->num_philos; i++)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal = data->start_time;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		data->philos[i].data = data;
	}
	return (0);
}

void	cleanup(t_data *data)
{
	for (int i = 0; i < data->num_philos; i++)
		pthread_mutex_destroy(&data->forks[i]);
	pthread_mutex_destroy(&data->write_lock);
	pthread_mutex_destroy(&data->death_lock);
	free(data->forks);
	free(data->philos);
}
