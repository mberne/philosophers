/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 18:47:04 by mberne            #+#    #+#             */
/*   Updated: 2021/11/22 17:09:40 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_death(t_structs *s)
{
	size_t	i;
	time_t	time_eat;

	while (!s->stop)
	{
		i = 0;
		while (i < s->num_philo)
		{
			gettimeofday(&s->now, NULL);
			pthread_mutex_lock(&s->philo[i].meal_protect);
			time_eat = (s->now.tv_usec / 1000 - s->philo[i].last_meal.tv_usec / 1000)
					+ (s->now.tv_sec * 1000 - s->philo[i].last_meal.tv_sec * 1000);
			pthread_mutex_unlock(&s->philo[i].meal_protect);
			if (time_eat > s->time_to_die)
			{
				print_status(s, s->philo[i].index, DIE);
				s->stop = 1;
				pthread_mutex_unlock(&s->speak);
				break ;
			}
			i++;
		}
	}
}

void	print_status(t_structs *s, int philo, t_action action)
{
	time_t	time;

	pthread_mutex_lock(&s->speak);
	if (s->stop)
	{
		pthread_mutex_unlock(&s->speak);
		return ;
	}
	gettimeofday(&s->now, NULL);
	time = (s->now.tv_usec / 1000 - s->beginning.tv_usec / 1000)
		+ (s->now.tv_sec * 1000 - s->beginning.tv_sec * 1000);
	printf("%ld %d ", time, philo);
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
	if (action != DIE)
		pthread_mutex_unlock(&s->speak);
}

void	wait_action(t_structs *s, int philo, t_action action, int action_time)
{
	struct timeval	start_time;
	time_t			actual_time;

	actual_time = 0;
	print_status(s, philo, action);
	gettimeofday(&start_time, NULL);
	while (actual_time < action_time && !s->stop)
	{
		usleep(100);
		actual_time = (s->now.tv_sec * 1000 - start_time.tv_sec * 1000);
		actual_time += (s->now.tv_usec / 1000 - start_time.tv_usec / 1000);
	}
}

size_t	synchro_threads_and_find_fork(t_philo *philo)
{
	while (!philo->s->wait_threads)
		usleep(20);
	gettimeofday(&philo->last_meal, NULL);
	if (philo->index % 2 == 0)
		usleep(2000);
	if (philo->index == philo->s->num_philo)
		return (0);
	else
		return (philo->index);
}

void	*routine(void *arg)
{
	t_philo	*philo;
	size_t	second_fork;

	philo = (t_philo *)arg;
	second_fork = synchro_threads_and_find_fork(philo);
	while (!philo->s->stop)
	{
		pthread_mutex_lock(&philo->s->fork[philo->index - 1]);
		print_status(philo->s, philo->index, TAKE_FORK);
		if (&philo->s->fork[philo->index - 1] == &philo->s->fork[second_fork])
			break ;
		pthread_mutex_lock(&philo->s->fork[second_fork]);
		print_status(philo->s, philo->index, TAKE_FORK);
		pthread_mutex_lock(&philo->meal_protect);
		gettimeofday(&philo->last_meal, NULL);
		pthread_mutex_unlock(&philo->meal_protect);
		wait_action(philo->s, philo->index, EAT, philo->s->time_to_eat);
		pthread_mutex_unlock(&philo->s->fork[philo->index - 1]);
		pthread_mutex_unlock(&philo->s->fork[second_fork]);
		wait_action(philo->s, philo->index, SLEEP, philo->s->time_to_sleep);
		print_status(philo->s, philo->index, THINK);
	}
	return (NULL);
}
