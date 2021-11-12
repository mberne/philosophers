/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 15:26:02 by mberne            #+#    #+#             */
/*   Updated: 2021/11/12 18:34:50 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_struct(t_structs *s)
{
	int	i;

	i = 0;
	while (i < s->num_philo)
	{
		pthread_mutex_destroy(&s->fork[i]);
		i++;
	}
	free(s->fork);
	free(s->philo);
}

void	create_philo(t_structs *s)
{
	int	i;

	i = 0;
	while (i < s->num_philo)
	{
		pthread_create(&s->philo[i].identifier, NULL, &routine, &s->philo[i]);
		i++;
		s->wait_threads++;
	}
	gettimeofday(&s->beginning, NULL);
	wait_death(s);
	i = 0;
	while (i < s->num_philo)
	{
		pthread_join(s->philo[i].identifier, NULL);
		i++;
	}
}

void	init_mutex(t_structs *s)
{
	int	i;

	i = 0;
	while (i < s->num_philo)
	{
		pthread_mutex_init(&s->fork[i], NULL);
		s->philo[i].index = i + 1;
		s->philo[i].s = s;
		i++;
	}
	pthread_mutex_init(&s->speak, NULL);
}

int	init_struct(t_structs *s, char **av)
{
	s->num_philo = ft_atoi(av[1]);
	s->time_to_die = ft_atoi(av[2]);
	s->time_to_eat = ft_atoi(av[3]);
	s->time_to_sleep = ft_atoi(av[4]);
	if (av[5])
		s->num_eat = ft_atoi(av[5]);
	else
		s->num_eat = INT32_MAX;
	if (s->num_philo == 0 || s->time_to_die == 0 || s->time_to_eat == 0
		|| s->time_to_sleep == 0 || s->num_eat == 0)
		return (-1);
	s->philo = ft_calloc(sizeof(t_philo), s->num_philo);
	s->fork = ft_calloc(sizeof(pthread_mutex_t), s->num_philo);
	if (!s->philo || !s->fork)
		return (-1);
	s->wait_threads = 0;
	s->death = 0;
	return (0);
}

int	check_args(int ac, char **av)
{
	size_t	i;
	char	*tmp;
	int		int_tmp;

	if (ac < 5 || ac > 6)
		return (-1);
	i = 1;
	while (av[i])
	{
		if (!str_isnumber(av[i]))
			return (-1);
		int_tmp = ft_atoi(av[i]);
		tmp = ft_itoa(int_tmp);
		if (!tmp || (tmp && ft_strcmp(av[i], tmp)))
		{
			free(tmp);
			return (-1);
		}
		free(tmp);
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_structs	s;

	if (check_args(ac, av) == -1 || init_struct(&s, av) == -1)
	{
		free_struct(&s);
		return (-1);
	}
	init_mutex(&s);
	create_philo(&s);
	free_struct(&s);
	return (0);
}
