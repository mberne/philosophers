/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 15:26:02 by mberne            #+#    #+#             */
/*   Updated: 2021/11/11 18:47:17 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_struct(t_structs *s)
{
	free(s->philo);
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
		s->num_eat = 0;
	s->philo = ft_calloc(s->num_philo, sizeof(t_philo));
	if (!s->philo)
		return (-1);
	if (gettimeofday(&s->beginning, NULL) == -1)
		return (-1);
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
	void		(*routine)(t_structs *);

	routine = philo_routine;
	if (check_args(ac, av) == -1 || init_struct(&s, av) == -1)
		return (-1);
	if (create_philo_and_forks(&s) == -1)
	{
		free_struct(&s);
		return (-1);
	}
	return (0);
}
