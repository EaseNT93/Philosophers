/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpants <dpants@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 17:14:47 by dpants            #+#    #+#             */
/*   Updated: 2021/12/15 17:14:47 by dpants           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

long int	get_time(void)
{
	struct timeval	time;
	long int		current_time;

	gettimeofday(&time, NULL);
	current_time = time.tv_sec * 1000 + time.tv_usec * 0.001;
	return (current_time);
}

void	print_msg(t_cfg *cfg, int phil_nb, char *msg)
{
	pthread_mutex_lock(&(cfg->msg));
	if (!cfg->die_f)
		printf("%li %i %s\n", get_time() - cfg->start_t, phil_nb, msg);
	pthread_mutex_unlock(&(cfg->msg));
}

int	philo_w8(t_cfg *cfg, long int start_time, long int w8_time)
{
	long int	cur_time;

	while (!cfg->die_f)
	{
		cur_time = get_time();
		if (cur_time - start_time >= w8_time)
			return (1);
		usleep(100);
	}
	return (0);
}
