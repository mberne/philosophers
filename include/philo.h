/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 15:19:07 by mberne            #+#    #+#             */
/*   Updated: 2021/11/11 19:10:03 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

/*** ~~ LIBS ~~ ***/

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
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
	pthread_t		identifier;
	pthread_mutex_t	fork;
	// int			index;
}					t_philo;

typedef struct s_structs
{
	size_t			num_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	size_t			num_eat;
	struct timeval	beginning;
	struct timeval	now;
	t_philo			*philo;
}				t_structs;

/*** ~~ PROTOTYPES ~~ ***/

// main.c
int		main(int ac, char **av);
int		check_args(int ac, char **av);
int		init_struct(t_structs *s, char **av);
void	free_struct(t_structs *s);

//routine.c
int		create_philo_and_forks(t_structs *s);
void	philo_routine(t_structs *s);
void	print_status(t_structs *s, int philosopher, t_action action);

// num_utils.c
char	*ft_itoa(int n);
int		ft_atoi(const char *str);
int		str_isnumber(char *s);

// str_utils.c
size_t	ft_strlen(const char *s);
int		ft_strcmp(const char *s1, const char *s2);
void	*ft_calloc(size_t count, size_t size);

#endif
