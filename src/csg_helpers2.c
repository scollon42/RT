/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   csg_helpers2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/27 09:26:39 by tbeauman          #+#    #+#             */
/*   Updated: 2016/05/02 11:24:07 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

double	save_lin_rin(t_env *e, t_ray *r, t_obj *dad)
{
	r->hit = vec3_add(r->pos, vec3_fmul(r->dir, dad->left->in));
	set_normal(e, r, dad->left);
	dad->normal = dad->left->normal;
	dad->mat = dad->left->mat;
	dad->in = dad->left->in;
	dad->out = dad->right->in;
	return (dad->left->in);
}

double	save_nothan(t_obj *dad)
{
	dad->in = INFINITY;
	dad->out = INFINITY;
	return (INFINITY);
}
