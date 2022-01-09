/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpants <dpants@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 15:25:07 by dpants            #+#    #+#             */
/*   Updated: 2022/01/09 14:31:51 by dpants           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	init_philo(t_cfg *cfg)
{
	int	i;

	cfg->philo = (t_philo *)malloc(sizeof(*(cfg->philo)) * cfg->philo_count);
	if (!cfg->philo)
		return (1);
	i = 0;
	while (cfg->philo_count > i)
	{
		cfg->philo[i].nbr = i + 1;
		cfg->philo[i].eat_count = 0;
		cfg->philo[i].last_eat_t = 0;
		cfg->philo[i].l_fork = i;
		cfg->philo[i].r_fork = (i + 1) % cfg->philo_count;
		cfg->philo[i].cfg = cfg;
		i++;
	}
	return (0);
}

int	init_mutex(t_cfg *cfg)
{
	int	i;

	cfg->fork_trd = malloc(sizeof(*(cfg->fork_trd)) * cfg->philo_count);
	if (!cfg->fork_trd)
		return (1);
	i = -1;
	while (cfg->philo_count > i++)
		if (pthread_mutex_init(&(cfg->fork_trd[i]), NULL))
			return (1);
	if (pthread_mutex_init(&(cfg->msg_trd), NULL))
		return (1);
	if (pthread_mutex_init(&(cfg->eat_trd), NULL))
		return (1);
	return (0);
}

int	init_config(t_cfg *cfg, int argc, char **argv)
{
	cfg->eat_count_f = argc == 6;
	cfg->philo_count = ft_atoi(argv[1]);
	cfg->die_t = ft_atoi(argv[2]);
	cfg->eat_t = ft_atoi(argv[3]);
	cfg->sleep_t = ft_atoi(argv[4]);
	if (cfg->eat_count_f)
		cfg->eat_count = ft_atoi(argv[5]);
	if (cfg->eat_count < 0 || cfg->philo_count < 0 || cfg->die_t < 0 || \
	cfg->eat_t < 0 || cfg->sleep_t < 0)
		return (1);
	if (init_mutex(cfg))
		return (1);
	if (init_philo(cfg))
		return (1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_cfg	cfg;

	if (argc != 5 && argc != 6)
		return (error_msg("Wrong number of arguments"));
	if (init_config(&cfg, argc, argv))
		return (error_msg("Config initialisation"));
	if (start_life_cycle(&cfg))
		return (error_msg("Thread create"));
	free(cfg.philo);
	free(cfg.fork_trd);
	return (0);
}
