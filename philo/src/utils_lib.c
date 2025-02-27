/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lib.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 10:29:58 by pledieu           #+#    #+#             */
/*   Updated: 2025/02/27 10:18:31 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

long long	get_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

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
