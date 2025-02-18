#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_data t_data;

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

struct s_data
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
};

// utils_lib.c
long long		get_timestamp(void);
void			ft_usleep(int ms);
int				ft_atoi(const char *str);
void			print_status(t_philo *philo, char *status);

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

#endif
