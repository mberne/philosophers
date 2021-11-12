/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 18:47:04 by mberne            #+#    #+#             */
/*   Updated: 2021/11/12 18:16:44 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_death(t_structs *s)
{
	int	i;

	while (1)
	{
		i = 0;
		while (i < s->num_philo)
		{
			if ((s->now.tv_usec / 1000) - s->philo[i].start > s->time_to_die)
			{
				print_status(s, s->philo[i].index, DIE);
				s->death = 1;
				break ;
			}
			i++;
		}
	}
}

void	print_status(t_structs *s, int philo, t_action action)
{
	int	time;

	pthread_mutex_lock(&s->speak);
	gettimeofday(&s->now, NULL);
	time = (s->now.tv_usec / 1000) - (s->beginning.tv_usec / 1000);
	printf("%d %d ", time, philo);
	if (action == TAKE_FORK)
		printf("has taken a fork\n");
	else if (action == EAT)
		printf("is eating\n");
	else if (action == SLEEP)
		printf("is sleeping\n");
	else if (action == THINK)
		printf("is thinking\n");
	else if (action == DIE)
		printf("died\n");
	pthread_mutex_unlock(&s->speak);
}

void	wait_action(t_structs *s, int philo, t_action action, int action_time)
{
	int	time;

	time = 0;
	print_status(s, philo, action);
	while (time < action_time)
	{
		usleep(100);
		time += 100;
	}
}

void	synchro_threads(t_philo *philo)
{
	while (philo->s->wait_threads != philo->s->num_philo)
		usleep(20);
	if (philo->index % 2 == 0)
		usleep(50);
}

void	*routine(void *arg)
{
	t_philo	*philo;
	int		index_second_fork;

	philo = (t_philo *)arg;
	if (philo->index == philo->s->num_philo)
		index_second_fork = 0;
	else
		index_second_fork = philo->index;
	synchro_threads(philo);
	while (!philo->s->death || !philo->s->num_eat)
	{
		pthread_mutex_lock(&philo->s->fork[philo->index - 1]);
		print_status(philo->s, philo->index, TAKE_FORK);
		pthread_mutex_lock(&philo->s->fork[index_second_fork]);
		print_status(philo->s, philo->index, TAKE_FORK);
		wait_action(philo->s, philo->index, EAT, philo->s->time_to_eat);
		pthread_mutex_unlock(&philo->s->fork[philo->index - 1]);
		pthread_mutex_unlock(&philo->s->fork[index_second_fork]);
		wait_action(philo->s, philo->index, SLEEP, philo->s->time_to_sleep);
		print_status(philo->s, philo->index, THINK);
		philo->s->num_eat--;
	}
	return (NULL);
}
