/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 15:26:02 by mberne            #+#    #+#             */
/*   Updated: 2021/11/10 19:28:35 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// void	print_status(t_structs *s, int i)
// {
// 	int	time;

// 	// time =
// 	printf("%d %d ", time, i);
// 	if ()
// 		printf("has taken a fork\n");
// 	else if ()
// 		printf("is eating\n");
// 	else if ()
// 		printf("is sleeping\n");
// 	else if ()
// 		printf("is thinking\n");
// 	else if ()
// 		printf("died\n");
// }

int	take_args(t_structs *s, char **av)
{
	size_t	i;
	char	*tmp;
	int		int_tmp;

	i = 1;
	while (av[i])
	{
		if (!str_isnumber(av[i]))
			return (-1);
		int_tmp = ft_atoi(av[i]);
		tmp = ft_itoa(int_tmp);
		if (tmp && ft_strcmp(av[i], tmp))
		{
			free(tmp);
			return (-1);
		}
		free(tmp);
		i++;
	}
	s->num_philo = ft_atoi(av[1]);
	s->time_to_die = ft_atoi(av[2]);
	s->time_to_eat = ft_atoi(av[3]);
	s->time_to_sleep = ft_atoi(av[4]);
	if (av[5])
		s->num_eat = ft_atoi(av[5]);
	else
		s->num_eat = 0;
	return (0);
}

int	main(int ac, char **av)
{
	t_structs	s;

	if (ac < 5 || ac > 6)
		return (-1);
	if (take_args(&s, av) == -1)
		return (-1);
	printf("%d %d %d %d %d\n", s.num_philo, s.time_to_die, s.time_to_eat, s.time_to_sleep, s.num_eat);
	return (0);
}
