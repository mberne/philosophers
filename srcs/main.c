/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 15:26:02 by mberne            #+#    #+#             */
/*   Updated: 2021/11/22 16:58:54 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_struct(t_structs *s)
{
	size_t	i;

	i = 0;
	while (i < s->num_philo)
	{
		pthread_mutex_destroy(&s->fork[i]);
		pthread_mutex_destroy(&s->philo[i].meal_protect);
		i++;
	}
	free(s->fork);
	free(s->philo);
}

int	create_philo(t_structs *s)
{
	size_t	i;

	i = 0;
	while (i < s->num_philo)
	{
		if (pthread_create(&s->philo[i].id, NULL, &routine, &s->philo[i]) > 0)
			return (-1);
		i++;
	}
	gettimeofday(&s->beginning, NULL);
	gettimeofday(&s->now, NULL);
	s->wait_threads = 1;
	usleep(100);
	wait_death(s);
	i = 0;
	while (i < s->num_philo)
	{
		if (pthread_join(s->philo[i].id, NULL) > 0)
			return (-1);
		i++;
	}
	return (0);
}

void	create_mutex_init_philo(t_structs *s)
{
	size_t	i;

	i = 0;
	while (i < s->num_philo)
	{
		pthread_mutex_init(&s->fork[i], NULL);
		s->philo[i].index = i + 1;
		s->philo[i].stop = 0;
		s->philo[i].dinner = 0;
		pthread_mutex_init(&s->philo[i].meal_protect, NULL);
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
	gettimeofday(&s->now, NULL);
	s->philo = ft_calloc(sizeof(t_philo), s->num_philo);
	s->fork = ft_calloc(sizeof(pthread_mutex_t), s->num_philo);
	if (!s->philo || !s->fork)
		return (-1);
	s->wait_threads = 0;
	s->stop = 0;
	return (0);
}

int	check_args(int ac, char **av)
{
	size_t	i;

	if (ac < 5 || ac > 6)
		return (-1);
	i = 1;
	while (av[i])
	{
		if (!str_isnumber(av[i]))
			return (-1);
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_structs	s;

	if (check_args(ac, av) == -1)
		return (-1);
	if (init_struct(&s, av) == -1)
	{
		free_struct(&s);
		return (-1);
	}
	create_mutex_init_philo(&s);
	if (create_philo(&s) == -1)
	{
		free_struct(&s);
		return (-1);
	}
	free_struct(&s);
	return (0);
}
