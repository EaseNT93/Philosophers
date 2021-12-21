/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpants <dpants@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 15:24:02 by dpants            #+#    #+#             */
/*   Updated: 2021/11/16 15:24:02 by dpants           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <pthread.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <semaphore.h>

typedef pthread_mutex_t mutex_t;

typedef struct s_philo
{
	int			nbr;
	int			eat_count;
	long int	last_eat_t;
	int			l_fork;
	int			r_fork;
	pthread_t	tread_id;
	t_cfg		*cfg;
}				t_philo;

typedef struct s_config
{
	int			eat_count;
	int			eat_count_f;
	int			philo_count;
	long int	eat_t;
	long int	sleep_t;
	long int	die_t;
	int			die_f;
	t_philo		*philo;
	mutex_t		*fork_trd;
	mutex_t		msg_trd;
	mutex_t		eat_trd;
}			t_cfg;

int	ft_strlen(char *str);
int	error_msg(char *str)
int	ft_atoi(char *str);

long int	get_time(void);
void		print_msg(t_cfg *cfg, int phil_nb, char *msg);
int			philo_w8(t_cfg *cfg, long int start_time, long int w8_time);

int	start_life_cycle(t_cfg *cfg);

#endif
