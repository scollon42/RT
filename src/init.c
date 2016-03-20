/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/11 14:46:31 by wwatkins          #+#    #+#             */
/*   Updated: 2016/03/20 06:44:30 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	init_env(t_env *e)
{
	e->reflect.depth_max = 4;
	e->refract.depth_max = 4;
	e->arg.w = (e->arg.w < 320 || e->arg.w > 10000 ? 1000 : e->arg.w);
	e->arg.h = (e->arg.h < 200 || e->arg.h > 5000 ? 900 : e->arg.h);
	e->win.w = e->arg.w;
	e->win.h = e->arg.h;
	e->win.dw = e->win.w / 2;
	e->win.dh = e->win.h / 2;
	if (!(e->win.adr = mlx_new_window(e->mlx, e->win.w, e->win.h, e->arg.file)))
		error(E_WIN_INIT, NULL, 1);
	e->intersect[SPHERE] = intersect_sphere;
	e->intersect[CONE] = intersect_cone;
	e->intersect[PLANE] = intersect_plane;
	e->intersect[CYLINDER] = intersect_cylinder;
	e->intersect[TRIANGLE] = intersect_triangle;
	e->intersect[ELLIPSOID] = intersect_ellipsoid;
	e->intersect[HYPERBOLOID_ONE] = intersect_hyperboloid1;
	e->intersect[HYPERBOLOID_TWO] = intersect_hyperboloid2;
	e->intersect[PARABOLOID] = intersect_paraboloid;
	e->intersect[TORUS] = intersect_torus;
	e->intersect[CUBE_TROUE] = intersect_cube_troue;
}

void	init_cam(t_env *e, t_cam *cam)
{
	double	w;
	double	h;
	double  coeff;

	e->cam->aa.supersampling = 1;
	e->cam->aa.inc = 1.0 / e->cam->aa.supersampling;
	e->cam->aa.coef = 1.0 / powf(e->cam->aa.supersampling, 2);
	coeff = (e->win.w < e->win.h ? e->win.w : e->win.h);
	w = e->win.w / coeff;
	h = e->win.h / coeff;
	cam->dist = 1.0 / tan(cam->fov / 2.0 * DEG2RAD);
	cam->origin = vec3_sub(vec3_add(vec3(0, 0, 0),
				vec3_add(vec3_fmul(vec3(0, 0, 1), cam->dist),
				vec3_fmul(vec3_up(), h / 2.0))),
				vec3_fmul(vec3_right(), w / 2.0));
	cam->xa = w / (double)e->win.w;
	cam->ya = h / (double)e->win.h;
}

void	init_key(t_env *e)
{
	e->key.up = 0;
	e->key.down = 0;
	e->key.left = 0;
	e->key.right = 0;
	e->key.invert = 0;
	e->key.gray_scale = 0;
	e->key.gamma_m = 0;
	e->key.gamma_p = 0;
	e->key.scale_p = 0;
	e->key.scale_m = 0;
}
