/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracing_intersect.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wwatkins <wwatkins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/11 14:42:27 by wwatkins          #+#    #+#             */
/*   Updated: 2016/05/03 10:43:51 by wwatkins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	transform_ray(t_ray *tray, t_obj *obj)
{
	tray->pos = vec3_sub(tray->pos, obj->pos);
	vec3_inverse_rotate(&tray->pos, obj->rot);
	vec3_inverse_rotate(&tray->dir, obj->rot);
}

void	save_obj(t_obj **out, t_obj *obj, double *tmin, double t)
{
	*out = obj;
	*tmin = t;
	(*out)->t = t;
}

t_obj	*intersect_object(t_env *e, t_ray *ray, double *tmin, t_obj *obj)
{
	t_obj	*out;
	double	t;
	t_obj	*r_value;
	t_ray	tray;

	out = NULL;
	while (obj != NULL)
	{
		tray = *ray;
		if (obj->type != CUBE && obj->type != CSG && obj->type != BBOX)
			transform_ray(&tray, obj);
		t = (obj->type == CSG ? intersect_csg(e, ray, obj) :
			e->intersect[obj->type](&tray, obj));
		if (t > EPSILON && t < *tmin)
		{
			if (obj->type == BBOX &&
				(r_value = intersect_object(e, ray, tmin, obj->comp)))
				out = r_value;
			else
				save_obj(&out, obj, tmin, t);
		}
		obj = obj->next;
	}
	return (out);
}

void	set_normal(t_env *e, t_ray *ray, t_obj *obj)
{
	t_vec3	tr;

	if (obj->type != CSG)
	{
		tr = vec3_sub(ray->hit, obj->pos);
		vec3_inverse_rotate(&tr, obj->rot);
		obj->normal = e->normal[obj->type](&tr, obj);
		vec3_rotate(&obj->normal, obj->rot);
		vec3_normalize(&obj->normal);
		obj->mat.texture.normal = obj->normal;
	}
}
