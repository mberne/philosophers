/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 15:26:02 by mberne            #+#    #+#             */
/*   Updated: 2021/11/26 16:56:51 by mberne           ###   ########lyon.fr   */
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
	pthread_mutex_destroy(&s->speak);
	free(s->fork);
	free(s->philo);
}

int	create_mutex_init_philo(t_structs *s, char **av)
{
	size_t	i;

	i = 0;
	while (i < s->num_philo)
	{
		if (pthread_mutex_init(&s->fork[i], NULL))
			return (-1);
		s->philo[i].index = i + 1;
		if (av[5])
			s->philo[i].num_eat = ft_atoi(av[5]);
		else
			s->philo[i].num_eat = INT32_MAX;
		if (pthread_mutex_init(&s->philo[i].meal_protect, NULL))
			return (-1);
		s->philo[i].s = s;
		i++;
	}
	if (pthread_mutex_init(&s->speak, NULL))
		return (-1);
	return (0);
}

int	init_struct(t_structs *s, char **av)
{
	s->num_philo = ft_atoi(av[1]);
	s->time_to_die = ft_atoi(av[2]);
	s->time_to_eat = ft_atoi(av[3]);
	s->time_to_sleep = ft_atoi(av[4]);
	if (s->num_philo == 0 || s->time_to_die == 0 || s->time_to_eat == 0
		|| s->time_to_sleep == 0)
		return (-1);
	gettimeofday(&s->now, NULL);
	s->philo = malloc(sizeof(t_philo) * s->num_philo);
	s->fork = malloc(sizeof(pthread_mutex_t) * s->num_philo);
	if (!s->philo || !s->fork)
		return (-1);
	s->wait_threads = 0;
	s->stop = 0;
	return (0);
}

int	check_args(int ac, char **av)
{
	size_t	i;
	char	*tmp;
	int		tmp_num;

	if (ac < 5 || ac > 6)
		return (-1);
	i = 1;
	while (av[i])
	{
		tmp_num = ft_atoi(av[i]);
		tmp = ft_itoa(tmp_num);
		if (tmp_num <= 0 || !tmp || ft_strcmp(tmp, av[i]))
		{
			free(tmp);
			return (-1);
		}
		free(tmp);
		tmp = NULL;
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_structs	s;

	if (check_args(ac, av) == -1)
		return (-1);
	if (init_struct(&s, av) == -1 || create_mutex_init_philo(&s, av) == -1)
	{
		free(s.fork);
		free(s.philo);
		return (-1);
	}
	if (create_philo(&s) == -1)
	{
		free_struct(&s);
		return (-1);
	}
	free_struct(&s);
	return (0);
}
