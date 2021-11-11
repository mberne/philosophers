/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 18:47:04 by mberne            #+#    #+#             */
/*   Updated: 2021/11/11 19:07:33 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_structs *s, int philosopher, t_action action)
{
	int	time;

	time = (s->now.tv_usec / 1000) - (s->beginning.tv_usec / 1000);
	printf("%d %d ", time, philosopher);
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
}

void	philo_routine(t_structs *s)
{
	(void)s;
	return ;
}

int	create_philo_and_forks(t_structs *s)
{
	size_t	i;

	i = 0;
	while (i < s->num_philo)
	{
		if (pthread_create(s->philo[i].identifier, NULL, routine(), s))
			return (-1);
		if (pthread_mutex_init(s->philo[i].fork, NULL))
			return (-1);
		i++;
	}
	return (0);
}
