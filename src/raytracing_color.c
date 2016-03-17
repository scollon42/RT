/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracing_color.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scollon <scollon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/11 14:28:29 by wwatkins          #+#    #+#             */
/*   Updated: 2016/03/17 10:17:41 by scollon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_vec3	raytracing_color(t_env *e, t_ray *ray, t_obj *obj)
{
	t_lgt	*light;
	t_vec3	color;
	t_vec3	diffuse;
	t_vec3	specular;

	color = (t_vec3) {0, 0, 0};
	light = e->lgt;
	while (light != NULL)
	{
		set_light(ray->hit, light);
		if (obj->mat.texture.defined)
			obj->mat.color = texture_mapping(obj, ray->hit);
		color = vec3_add(color, vec3_fmul(light->color,
				obj->mat.ambient));// - 0.005 * obj->t));
		diffuse = set_diffuse(obj, light);
		specular = set_specular(e, ray->hit, obj, light);
		color = vec3_add(color, vec3_add(diffuse, specular));
		color = vec3_mul(color, obj->mat.color);
		color = vec3_fmul(color, light->intensity);
		obj->mat.shadow ? set_shadow(e, &color, *light, obj) : 0;
		light = light->next;
	}
	return (color);
}

t_vec3	set_diffuse(t_obj *obj, t_lgt *light)
{
	double	diff;
	
	diff = vec3_dot(light->ray.dir, obj->normal);
	diff < 0 ? diff = 0 : 0;
	return (vec3_fmul(light->color, diff * obj->mat.diffuse));
	
	// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading/
	// better shading model, light intensity and distance is taken into account, 
	// light->intensity multiplication in raytracing_color function can be deleted,
	// obj->t is the distance from light
	/*double	diff;

	diff = ft_clampf(vec3_dot(light->ray.dir, obj->normal), 0, 1);
	return (vec3_fmul(light->color, obj->mat.diffuse * light->intensity * diff / (obj->t * obj->t)));*/
}

t_vec3	set_specular(t_env *e, t_vec3 hit, t_obj *obj, t_lgt *light)
{
	t_vec3	lighdir;
	t_vec3	viewdir;
	t_vec3	halfdir;
	float	spec;

	lighdir = vec3_norm(vec3_sub(light->pos, hit));
	viewdir = vec3_norm(vec3_sub(e->cam->pos, hit));
	halfdir = vec3_norm(vec3_add(lighdir, viewdir));
	spec = pow(vec3_dot(obj->normal, halfdir), obj->mat.shininess);
	return (vec3_fmul(light->color, spec * obj->mat.specular));
}

void	set_light(t_vec3 hit, t_lgt *light)
{
	light->ray.pos = hit;
	light->ray.dir = vec3_sub(light->pos, hit);
	vec3_normalize(&light->ray.dir);
}

void	set_shadow(t_env *e, t_vec3 *color, t_lgt light, t_obj *obj)
{
	double	tmin;

	tmin = INFINITY;
	light.ray.dir = vec3_norm(vec3_sub(light.ray.pos, light.pos));
	light.ray.pos = light.pos;
	if (intersect_object(e, &light.ray, &tmin) != obj)
		*color = vec3_fmul(*color, 0.5);
}
