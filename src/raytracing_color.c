/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracing_color.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wwatkins <wwatkins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/11 14:28:29 by wwatkins          #+#    #+#             */
/*   Updated: 2016/03/11 16:54:32 by wwatkins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_vec3	raytracing_color(t_env *e, t_ray ray, t_obj *obj)
{
	t_lgt	*light;
	t_vec3	color;
	t_vec3	diffuse;
	t_vec3	specular;

	color = (t_vec3) {0, 0, 0};
	light = e->light;
	while ((light = light->next) != NULL)
	{
		set_light(e, ray.hit, light);
		color = vec3_fmul(light->color, obj->mat.ambient);
		diffuse = set_diffuse(e, obj, light);
		specular = set_specular(e, ray.hit, obj, light);
		color = vec3_add(color, vec3_add(diffuse, specular));
		set_shadow(e, &color, light->ray, light);
	}
	return (color);
}

t_vec3	set_diffuse(t_env *e, t_obj *obj, t_lgt *light)
{
	double	diff;

	diff = vec3_dot(light->ray.dir, obj->normal);
	diff < 0.0 ? diff = 0.0 : 0;
	return (vec3_fmul(light->color, diff * obj->mat.diffuse));
}

t_vec3	set_specular(t_env *e, t_vec3 hit, t_obj *obj, t_lgt *light)
{
	t_vec3	lightdir;
	t_vec3	viewdir;
	t_vec3	halfdir;
	float	spec;

	lighdir = vec3_norm(vec3_sub(light->pos, hit));
	viewdir = vec3_norm(vec3_sub(e->cam.pos, hit));
	halfdir = vec3_norm(vec3_add(lightdir, viewdir));
	spec = pow(vec3_dot(obj->normal, halfdir), obj->mat.shininess);
	return (vec3_fmul(light->color, spec * obj->mat.specular));
}

void	set_light(t_env *e, t_vec3 hit, t_lgt *light)
{
	light->ray->pos = light->pos;
	light->ray->dir = vec3_sub(hit, light->pos);
	vec3_normalize(&light->ray->dir);
}

void	set_shadow(t_env *e, t_vec3 *color, t_ray ray, t_obj *obj)
{
	double	tmin;

	tmin = INFINITY;
	if (intersect_object(e, ray, &tmin) != obj)
		*color = vec3_fmul(*color, 0.5);
}
