/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alistair <alistair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 23:26:03 by alistair          #+#    #+#             */
/*   Updated: 2022/04/02 23:50:31 by alistair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char *argv[])
{
	long	n;
	long	ttd;
	long	tte;
	long	tts;

	if (argc >= 4 && argc <= 5)
	{
		n = ft_atoi(argv[1]);
		ttd = ft_atoi(argv[2]);
		tte = ft_atoi(argv[3]);
		tts = ft_atoi(argv[4]);
	}
}