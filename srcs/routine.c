/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 18:47:04 by mberne            #+#    #+#             */
/*   Updated: 2021/11/14 19:24:14 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_death(t_structs *s)
{
	int	i;
	int	test;

	while (!s->death)
	{
		i = 0;
		while (i < s->num_philo)
		{
			gettimeofday(&s->now, NULL);
			pthread_mutex_lock(&s->philo[i].meal_protect);
			test = ((s->now.tv_usec - s->philo[i].last_meal.tv_usec) / 1000 + \
			(s->now.tv_sec - s->philo[i].last_meal.tv_sec) * 1000);
			pthread_mutex_unlock(&s->philo[i].meal_protect);
			if (test > s->time_to_die)
			{
				print_status(s, s->philo[i].index, DIE);
				s->death = 1;
				pthread_mutex_unlock(&s->speak);
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
	if (s->death)
	{
		pthread_mutex_unlock(&s->speak);
		return ;
	}
	gettimeofday(&s->now, NULL);
	time = (s->now.tv_usec / 1000 - s->beginning.tv_usec / 1000)
		+ (s->now.tv_sec * 1000 - s->beginning.tv_sec * 1000);
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
	if (action != DIE)
		pthread_mutex_unlock(&s->speak);
}

void	wait_action(t_structs *s, int philo, t_action action, int action_time)
{
	struct timeval	start_time;
	int				actual_time;

	actual_time = 0;
	print_status(s, philo, action);
	gettimeofday(&start_time, NULL);
	while (actual_time < action_time && !s->death)
	{
		usleep(10);
		actual_time = (s->now.tv_sec * 1000 - start_time.tv_sec * 1000);
		actual_time += (s->now.tv_usec / 1000 - start_time.tv_usec / 1000);
	}
}

void	synchro_threads(t_philo *philo)
{
	while (!philo->s->wait_threads)
		usleep(20);
	gettimeofday(&philo->last_meal, NULL);
	if (philo->index % 2 == 0)
		usleep(2000);
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
	while (!philo->s->death)
	{
		pthread_mutex_lock(&philo->s->fork[philo->index - 1]);
		print_status(philo->s, philo->index, TAKE_FORK);
		pthread_mutex_lock(&philo->s->fork[index_second_fork]);
		print_status(philo->s, philo->index, TAKE_FORK);
		pthread_mutex_lock(&philo->meal_protect);
		gettimeofday(&philo->last_meal, NULL);
		pthread_mutex_unlock(&philo->meal_protect);
		wait_action(philo->s, philo->index, EAT, philo->s->time_to_eat);
		pthread_mutex_unlock(&philo->s->fork[philo->index - 1]);
		pthread_mutex_unlock(&philo->s->fork[index_second_fork]);
		wait_action(philo->s, philo->index, SLEEP, philo->s->time_to_sleep);
		print_status(philo->s, philo->index, THINK);
		philo->s->num_eat--;
	}
	return (NULL);
}
