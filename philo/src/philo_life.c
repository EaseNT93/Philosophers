/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpants <dpants@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 16:58:00 by dpants            #+#    #+#             */
/*   Updated: 2022/01/09 19:04:24 by dpants           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	philo_eat(t_philo *philo)
{
	t_cfg	*cfg;

	cfg = malloc(sizeof(t_cfg));
	//printf("%p\n", cfg);
	cfg = philo->cfg;
	//printf("start_PE\n");
	pthread_mutex_lock(&(cfg->fork_trd[philo->r_fork]));
	print_msg(cfg, philo->nbr, "has taken a right fork");
	if (cfg->philo_count > 1)
	{
		pthread_mutex_lock(&(cfg->fork_trd[philo->l_fork]));
		print_msg(cfg, philo->nbr, "has taken a left fork");
		//printf("start_PE1, die_f = %d\n", cfg->die_f);
		//pthread_mutex_lock(&(cfg->eat_trd)); // no need
		//printf("start_PE2, die_f = %d\n", cfg->die_f);
		print_msg(cfg, philo->nbr, "is eating");
		//printf("start_PE3, die_f = %d\n", cfg->die_f);
		philo->last_eat_t = get_time();
		philo->eat_count++;
		//pthread_mutex_unlock(&(cfg->eat_trd));
		philo_w8(cfg, get_time(), cfg->eat_t);
		pthread_mutex_unlock(&(cfg->fork_trd[philo->l_fork]));
	}
	pthread_mutex_unlock(&(cfg->fork_trd[philo->r_fork]));
	//printf("end_PE\n");
}

void	philo_death(t_cfg *cfg) // need full rework (!)
{
	long int	cur_time;
	int			i;

	//printf("start_PD\n");
	while (!cfg->die_f && cfg->eat_count_f != 2)
	{
		i = -1;
		cur_time = get_time();
		//printf("start_PD1\n");
		while (!cfg->die_f && cfg->philo_count > i++)
		{
			//pthread_mutex_lock(&(cfg->eat_trd));
			if (cur_time - cfg->philo[i].last_eat_t > cfg->die_t) // not like this
			{
				print_msg(cfg, i + 1, "dies");
				cfg->die_f++;
			}
			//pthread_mutex_unlock(&(cfg->eat_trd));
		}
	}
}

void	*philo_life(void *philo)
{
	t_philo	*cur_phil;
	t_cfg	*cfg;
	int		i;

	//printf("start_PL\n");
	cur_phil = (t_philo *)philo;
	cfg = cur_phil->cfg;
	if (cfg->philo_count > 1)
		if (cur_phil->nbr % 2)
			usleep(10000);
	while (!cfg->die_f && cfg->eat_count_f != 2)
	{
		philo_eat(cur_phil);
		i = 0;
		//printf("start_PL1 & eat_f = %i\n", cfg->eat_count_f);
		if (cfg->eat_count_f)
			while (cfg->philo_count > i && \
			cfg->philo[i].eat_count == cfg->eat_count)
				i++;
		if (i >= cfg->philo_count)
			cfg->eat_count_f = 2;
		//printf("start_PL3 & eat_f = %i\n", cfg->eat_count_f);
		if (cfg->philo_count == 1 || cfg->eat_count_f == 2)
			break ;
		print_msg(cfg, cur_phil->nbr, "is sleeping");
		philo_w8(cfg, get_time(), cfg->sleep_t);
		print_msg(cfg, cur_phil->nbr, "is thinking");
	}
	//printf("end_PL\n");
	return (NULL);
}

int	start_life_cycle(t_cfg *cfg)
{
	t_philo	*philo;
	int		i;

	//printf("start_PLC\n");
	philo = cfg->philo;
	cfg->start_t = get_time();
	i = 0;
	while (cfg->philo_count > i)
	{
		if (pthread_create(&(philo[i].trd_id), NULL, philo_life, &(philo[i])))
			return (1);
		philo[i++].last_eat_t = get_time();
	}
	//printf("start_PLC1\n");
	philo_death(cfg); // not here
	//printf("start_PLC2\n");
	i = 0;
	while (cfg->philo_count > i)
		pthread_join(philo[i++].trd_id, NULL);
	i = 0;
	while (cfg->philo_count > i)
		pthread_mutex_destroy(&(cfg->fork_trd[i++]));
	pthread_mutex_destroy(&(cfg->msg_trd));
	//printf("end_PLC\n");
	return (0);
}
