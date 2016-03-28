/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracing_recursion.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wwatkins <wwatkins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/12 15:55:04 by wwatkins          #+#    #+#             */
/*   Updated: 2016/03/27 15:21:11 by wwatkins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_vec3		raytracing_reflect(t_env *e, t_ray ray, t_obj *obj)
{
	t_vec3	color;

	color = (t_vec3) {0, 0, 0};
	if (e->reflect.depth < e->reflect.depth_max)
	{
		e->reflect.depth++;
		ray.pos = ray.hit;
		if (obj->mat.texture.normal_map)
			ray.dir = vec3_reflect(ray.dir, obj->mat.texture.normal);
		else
			ray.dir = vec3_reflect(ray.dir, obj->normal);
		color = vec3_add(color, raytracing_draw(e, ray));
		color = vec3_fmul(color, obj->mat.reflect);
		if (obj->mat.fresnel.defined)
			color = vec3_fmul(color, get_fresnel(vec3_fmul(e->cam->ray.dir, -1),
			obj->normal, obj->mat.fresnel.reflect));
	}
	return (color);
}

// Work in progress
// static t_vec3	glossy_reflection_direction(t_ray ray, t_obj *obj)
// {
// 	t_vec3	dir;
// 	double	a = rand() / (double)RAND_MAX;
// 	double	b = rand() / (double)RAND_MAX;
// 	double	phi = pow(acos(1.0 - a), 1.0);
// 	double	theta = 2.0 * M_PI * b;
// 	double	x = sin(phi) * cos(theta);
// 	double	y = sin(phi) * sin(theta);
// 	double	z = cos(phi);
// 	t_vec3	w = ray.dir;
// 	t_vec3	u = vec3_cross(w, obj->normal);
// 	t_vec3	v = vec3_cross(w, u);
// 	dir = vec3_add(vec3_fmul(u, x), vec3_add(vec3_fmul(v, y), vec3_fmul(w, z)));
// 	return (vec3_norm(dir));
// }
//
// t_vec3		raytracing_reflect(t_env *e, t_ray ray, t_obj *obj)
// {
// 	t_vec3	color;
// 	t_vec3	tmp;
//
// 	color = (t_vec3) {0, 0, 0};
// 	if (e->reflect.depth < e->reflect.depth_max)
// 	{
// 		e->reflect.depth++;
// 		ray.pos = ray.hit;
// 		tmp = vec3_reflect(ray.dir, obj->normal);
// 		ray.dir = tmp;
// 		for (int z = 0; z < 120; z++)
// 		{
// 			ray.dir = glossy_reflection_direction(ray, obj);
// 			color = vec3_add(color, raytracing_draw(e, ray));
// 			ray.dir = tmp;
// 			// color = vec3_fmul(color, obj->mat.reflect);
// 		}
// 		color = vec3_fmul(color, 1.0 / 120.0);
// 	}
// 	return (color);
// }

static void	refract_dir(t_env *e, t_ray *ray, t_obj *obj)
{
	double	n;
	double	cosI;
	double	cosT;
	double	sinT2;
	t_vec3	normal;

	ray->pos = ray->hit;
	if (obj->mat.texture.normal_map)
		normal = obj->mat.texture.normal;
	else
		normal = obj->normal;
	cosI = vec3_dot(normal, ray->dir);
	if (cosI > 0.0)
	{
		e->refract.n1 = obj->mat.refract;
		e->refract.n2 = 1.0;
		normal = vec3_fmul(normal, -1.0);
	}
	else
	{
		e->refract.n1 = 1.0;
		e->refract.n2 = obj->mat.refract;
		cosI = -cosI;
	}
	n = e->refract.n1 / e->refract.n2;
	sinT2 = n * n * (1.0 - cosI * cosI);
	cosT = sqrt(1.0 - sinT2);
	ray->dir = vec3_add(vec3_fmul(ray->dir, n),
	vec3_fmul(normal, (n * cosI - cosT)));
	vec3_normalize(&ray->dir);
}

t_vec3		raytracing_refract(t_env *e, t_ray ray, t_obj *obj)
{
	t_vec3	color;

	color = (t_vec3) {0, 0, 0};
	if (e->refract.depth < e->refract.depth_max)
	{
		e->refract.depth++;
		refract_dir(e, &ray, obj);
		color = vec3_add(color, vec3_fmul(raytracing_draw(e, ray),
			obj->mat.transparency));
		if (obj->mat.absorbtion < 1.0)
			color = vec3_fmul(color, powf(obj->mat.absorbtion, obj->scale));
		if (obj->mat.fresnel.defined)
			color = vec3_fmul(color, get_fresnel(vec3_fmul(e->cam->ray.dir, -1),
			obj->normal, obj->mat.fresnel.refract));
		// obj->scale * 2.0 is not correct, t is distance traced in object
	}
	return (color);
}
