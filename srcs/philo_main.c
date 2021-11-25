/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 15:44:40 by mberne            #+#    #+#             */
/*   Updated: 2021/11/25 15:45:59 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	wait_dinners(t_structs *s)
{
	size_t	i;
	size_t	dinners_done;

	i = 0;
	dinners_done = 0;
	while (i < s->num_philo)
	{
		if (s->philo[i].num_eat <= 0)
			dinners_done++;
		i++;
	}
	if (dinners_done >= s->num_philo)
	{
		s->stop = 1;
		return (1);
	}
	return (0);
}

void	wait_death(t_structs *s)
{
	size_t	i;
	time_t	eat;

	while (!s->stop)
	{
		i = 0;
		while (i < s->num_philo)
		{
			gettimeofday(&s->now, NULL);
			pthread_mutex_lock(&s->philo[i].meal_protect);
			eat = (s->now.tv_usec / 1000 - s->philo[i].last_meal.tv_usec / 1000)
				+ (s->now.tv_sec * 1000 - s->philo[i].last_meal.tv_sec * 1000);
			pthread_mutex_unlock(&s->philo[i].meal_protect);
			if (eat > s->time_to_die && s->philo[i].num_eat > 0)
			{
				print_status(s, s->philo[i].index, DIE);
				s->stop = 1;
				pthread_mutex_unlock(&s->speak);
				break ;
			}
			if (wait_dinners(s))
				break ;
			i++;
		}
	}
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
