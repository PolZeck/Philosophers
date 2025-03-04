/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 14:31:13 by pledieu           #+#    #+#             */
/*   Updated: 2025/02/27 18:07:15 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_data	t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
}	t_philo;

typedef struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_meals;
	int				simulation_running;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	death_lock;
	t_philo			*philos;
}	t_data;

// utils_lib.c
long long		get_timestamp(void);
void			ft_usleep(int time_in_ms, t_data *data);
int				ft_atoi(const char *nptr);
void			print_status(t_philo *philo, const char *status);

// init_and_parse.c
int				init_data(t_data *data, char **av);
int				init_philosophers(t_data *data);
void			cleanup(t_data *data);

// threads.c
void			*philosopher_routine(void *arg);
int				start_simulation(t_data *data);

// activity.c
void			eat(t_philo *philo);
void			sleep_and_think(t_philo *philo);
void			*monitor_death(void *arg);
void			*monitor_meals(void *arg);

void			take_fork_alone(t_philo *philo);
int				check_simulation_running(t_philo *philo);
int				create_philo_threads(t_data *data);
int				death_check(t_data *data, int i);
int				count_finished_meals(t_data *data);

#endif
