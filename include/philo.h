/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 15:19:07 by mberne            #+#    #+#             */
/*   Updated: 2021/11/22 16:57:20 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

/*** ~~ LIBS ~~ ***/

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/time.h>
# include <pthread.h>

/*** ~~ STRUCTURES ~~ ***/

typedef enum e_action
{
	TAKE_FORK,
	EAT,
	SLEEP,
	THINK,
	DIE,
}	t_action;

typedef struct s_philo
{
	pthread_t			id;
	size_t				index;
	bool				stop;
	size_t				dinner;
	struct timeval		last_meal;
	pthread_mutex_t		meal_protect;
	struct s_structs	*s;
}						t_philo;

struct s_structs
{
	size_t			num_philo;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	size_t			num_eat;
	struct timeval	beginning;
	struct timeval	now;
	t_philo			*philo;
	pthread_mutex_t	*fork;
	pthread_mutex_t	speak;
	bool			wait_threads;
	bool			stop;
};

typedef struct s_structs	t_structs;

/*** ~~ PROTOTYPES ~~ ***/

// main.c
int		main(int ac, char **av);
int		check_args(int ac, char **av);
int		init_struct(t_structs *s, char **av);
void	create_mutex_init_philo(t_structs *s);
int		create_philo(t_structs *s);
void	free_struct(t_structs *s);

//routine.c
void	*routine(void *arg);
size_t	synchro_threads_and_find_fork(t_philo *philo);
void	wait_action(t_structs *s, int philo, t_action action, int action_time);
void	print_status(t_structs *s, int philosopher, t_action action);
void	wait_death(t_structs *s);

// num_utils.c
char	*ft_itoa(int n);
int		ft_atoi(const char *str);

// str_utils.c
int		ft_strcmp(const char *s1, const char *s2);
void	*ft_calloc(size_t count, size_t size);
int		str_isnumber(char *s);

#endif
