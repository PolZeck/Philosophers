/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lib.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 10:29:58 by pledieu           #+#    #+#             */
/*   Updated: 2025/02/18 12:17:20 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/philosophers.h"

long long get_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(int ms)
{
	long long	start_time;

	start_time = get_timestamp();
	while ((get_timestamp() - start_time) < ms)
		usleep(100);
}

void	print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->data->write_lock);
	if (philo->data->simulation_running)  // 🔥 Vérifier avant d'afficher
		printf("%lld %d %s\n", get_timestamp() - philo->data->start_time, philo->id, status);
	pthread_mutex_unlock(&philo->data->write_lock);
}

