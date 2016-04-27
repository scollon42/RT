/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stereoscopy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scollon <scollon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 11:30:30 by scollon           #+#    #+#             */
/*   Updated: 2016/04/27 14:34:36 by scollon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	compute_cyan(t_img *stereo, t_img *img)
{
	int		x;
	int		y;
	int		pos;

	y = -1;
	while (++y < img->h)
	{
		x = -1;
		while (++x < img->w)
		{
			pos = (x * img->opp) + (y * img->sl);
			stereo->img[pos] += img->img[pos] & 0xFF;
			stereo->img[pos + 1] += img->img[pos + 1] & 0xFF;
		}
	}
}

static void	compute_red(t_img *stereo, t_img *img)
{
	int		x;
	int		y;
	int		pos;

	y = -1;
	while (++y < img->h)
	{
		x = -1;
		while (++x < img->w)
		{
			pos = (x * img->opp) + (y * img->sl);
			stereo->img[pos + 2] += img->img[pos + 2] & 0xFF;
		}
	}
}

void	generate_stereoscopy(t_env *e)
{
	e->cam->stereo = img_init(e);
	compute_cyan(&e->cam->stereo, &e->cam->twin->img);
	compute_red(&e->cam->stereo, &e->cam->img);
}
