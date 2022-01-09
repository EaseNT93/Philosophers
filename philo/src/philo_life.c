/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpants <dpants@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 16:58:00 by dpants            #+#    #+#             */
/*   Updated: 2022/01/09 14:32:06 by dpants           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	philo_eat(t_philo *philo)
{
	t_cfg	*cfg;

	cfg = philo->cfg;
	pthread_mutex_lock(&(cfg->fork_trd[philo->r_fork]));
	print_msg(cfg, philo->nbr, "has taken a right fork");
	if (cfg->philo_count > 1)
	{
		pthread_mutex_lock(&(cfg->fork_trd[philo->l_fork]));
		print_msg(cfg, philo->nbr, "has taken a left fork");
		pthread_mutex_lock(&(cfg->eat_trd));
		print_msg(cfg, philo->nbr, "is eating");
		philo->last_eat_t = get_time();
		philo->eat_count++;
		pthread_mutex_unlock(&(cfg->eat_trd));
		philo_w8(cfg, get_time(), cfg->eat_t);
		pthread_mutex_unlock(&(cfg->fork_trd[philo->l_fork]));
	}
	pthread_mutex_unlock(&(cfg->fork_trd[philo->r_fork]));
}

void	philo_death(t_cfg *cfg)
{
	int	i;

	while (!cfg->die_f)
	{
		i = 0;
		while (!cfg->die_f && cfg->philo_count > i)
		{
			pthread_mutex_lock(&(cfg->eat_trd));
			if (philo_w8(cfg, cfg->philo[i].last_eat_t, cfg->die_t))
			{
				print_msg(cfg, i + 1, "dies");
				cfg->die_f++;
			}
			pthread_mutex_unlock(&(cfg->eat_trd));
		}
		if (cfg->die_f)
			break ;
		i = 0;
		if (cfg->eat_count_f)
			while (cfg->philo_count > i && \
			cfg->philo[i].eat_count == cfg->eat_count)
				i++;
		if (i == cfg->philo_count)
			cfg->eat_count_f = 2;
	}
}

void	*philo_life(void *philo)
{
	t_philo	*cur_phil;
	t_cfg	*cfg;

	cur_phil = (t_philo *)philo;
	cfg = cur_phil->cfg;
	if (cfg->philo_count > 1)
		if (cur_phil->nbr % 2)
			usleep(10000);
	while (!cfg->die_f)
	{
		philo_eat(cur_phil);
		if (cfg->philo_count == 1 || cfg->eat_count_f == 2)
			break ;
		print_msg(cfg, cur_phil->nbr, "is sleeping");
		philo_w8(cfg, get_time(), cfg->sleep_t);
		print_msg(cfg, cur_phil->nbr, "is thinking");
	}
	return (NULL);
}

int	start_life_cycle(t_cfg *cfg)
{
	t_philo	*philo;
	int		i;

	philo = cfg->philo;
	cfg->start_t = get_time();
	i = 0;
	while (cfg->philo_count > i)
	{
		if (pthread_create(&(philo[i].trd_id), NULL, philo_life, &(philo[i])))
			return (1);
		philo[i++].last_eat_t = get_time();
	}
	philo_death(cfg);
	i = 0;
	while (cfg->philo_count > i)
		pthread_join(philo[i++].trd_id, NULL);
	i = 0;
	while (cfg->philo_count > i)
		pthread_mutex_destroy(&(cfg->fork_trd[i]));
	pthread_mutex_destroy(&(cfg->msg_trd));
	return (0);
}
