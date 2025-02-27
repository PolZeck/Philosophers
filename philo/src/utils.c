/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:11:57 by pledieu           #+#    #+#             */
/*   Updated: 2025/02/27 16:32:06 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

static int	ft_isdigit_str(const char *nptr)
{
	int	i;

	i = 0;
	while (nptr[i] >= '0' && nptr[i] <= '9')
		i++;
	if (nptr[i] != '\0')
		return (0);
	return (1);
}

int	ft_atoi(const char *nptr)
{
	int					i;
	int					sign;
	unsigned long long	ans;

	i = 0;
	sign = 1;
	ans = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == ' ')
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
		if (nptr[i++] == '-')
			sign *= -1;
	if (!(nptr[i] >= '0' && nptr[i] <= '9') || !ft_isdigit_str(&nptr[i]))
		return (-1);
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		ans = ans * 10 + (nptr[i++] - '0');
		if (ans > __LONG_LONG_MAX__)
			return (-1);
	}
	return (((int) ans) * sign);
}
/**
 * @brief Cleans up allocated resources and destroys mutexes.
 * 
 * This function:
 * - Iterates through all forks and destroys their mutexes.
 * - Destroys the `write_lock` and `death_lock` mutexes.
 * - Frees dynamically allocated memory for forks and philosophers.
 * 
 * @param data Pointer to the `t_data` structure containing 
 *             simulation data.
 * 
 * @note This function should be called at the end of the 
 *       simulation to prevent memory leaks.
 * @note Ensures that all mutexes are properly destroyed 
 *       before freeing memory.
 */

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
