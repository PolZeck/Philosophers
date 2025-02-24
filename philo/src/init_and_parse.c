/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_parse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 10:29:19 by pledieu           #+#    #+#             */
/*   Updated: 2025/02/24 15:46:21 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	ft_atoi(const char *nptr)
{
	int					i;
	int					sign;
	unsigned long long	ans;

	i = 0;
	sign = 1;
	ans = 0;
	while (((nptr[i] >= 9 && nptr[i] <= 13)) || nptr[i] == ' ')
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign *= -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		ans = ans * 10 + (nptr[i] - '0');
		if (ans >= __LONG_LONG_MAX__ && sign < 0)
			return (0);
		if (ans >= __LONG_LONG_MAX__)
			return (-1);
		i++;
	}
	return (((int) ans) * sign);
}

int	init_data(t_data *data, char **av)
{
	int	i;

	i = -1;
	data->num_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (av[5])
		data->num_meals = ft_atoi(av[5]);
	else
		data->num_meals = -1;
	if (data->num_philos <= 0 || data->time_to_die <= 0
		|| data->time_to_eat <= 0 || data->time_to_sleep <= 0)
		return (printf("Invalid arguments\n"), 1);
	data->simulation_running = 1;
	data->start_time = get_timestamp();
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->forks || !data->philos)
		return (printf("Memory allocation failed\n"), 1);
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

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->write_lock);
	pthread_mutex_destroy(&data->death_lock);
	free(data->forks);
	free(data->philos);
}
