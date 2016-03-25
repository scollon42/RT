/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wwatkins <wwatkins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/09 12:00:43 by wwatkins          #+#    #+#             */
/*   Updated: 2016/03/25 13:19:37 by wwatkins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	core(t_env *e)
{
	init_key(e);
	raytracing(e);
	mlx_hook(e->win.adr, 2, (1L << 0), key_pressed, e);
	mlx_hook(e->win.adr, 3, (1L << 1), key_released, e);
	mlx_hook(e->win.adr, 6, (1L << 6), mouse_pos, e);
	mlx_mouse_hook(e->win.adr, mouse_hook, e);
	mlx_expose_hook(e->win.adr, expose_hook, e);
	mlx_loop_hook(e->mlx, loop_hook, e);
	mlx_loop(e->mlx);
}
