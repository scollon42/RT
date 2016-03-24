/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_light.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wwatkins <wwatkins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/21 14:24:38 by scollon           #+#    #+#             */
/*   Updated: 2016/03/24 14:24:46 by wwatkins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		default_light(t_lgt *light)
{
	light->type = POINT;
	light->pos = vec3(0, 0, 0);
	light->dir = vec3(0, 0, 1);
	light->color = vec3(1, 1, 1);
	light->intensity = 1;
	light->attenuation = 0.1;
	light->cutoff = 45;
	light->shadow = HARD;
}

static int		get_light_type(char *line)
{
	if (ft_strstr(line, "POINT"))
		return (POINT);
	else if (ft_strstr(line, "DIRECTIONAL"))
		return (DIRECTIONAL);
	else if (ft_strstr(line, "SPOT"))
		return (SPOT);
	else
		error(E_LTYPE, line, 0);
	return (POINT);
}

static t_lgt	*create_light(t_env *e, t_line *light_line)
{
	t_lgt		*new;
	t_line		*line;

	line = light_line;
	!(new = (t_lgt*)malloc(sizeof(t_lgt))) ? error(E_LINIT, NULL, 1) : 0;
	default_light(new);
	while (line != NULL && !ft_strstr(line->line, "- light:"))
	{
		if (ft_strstr(line->line, "type:"))
			new->type = get_light_type(line->line);
		else if (ft_strstr(line->line, "pos:"))
			new->pos = parse_vector(line->line);
		else if (ft_strstr(line->line, "dir:"))
			new->dir = parse_vector(line->line);
		else if (ft_strstr(line->line, "color:"))
			new->color = parse_color(line->line);
		else if (ft_strstr(line->line, "intensity:"))
			new->intensity = parse_value(line->line, 0, 10);
		else if (ft_strstr(line->line, "attenuation:"))
			new->attenuation = parse_value(line->line, 0.0001, 10);
		else if (ft_strstr(line->line, "cutoff:"))
			new->cutoff = parse_value(line->line, 0, 360);
		else if (ft_strstr(line->line, "cutoffouter:"))
			new->cutoff_outer = parse_value(line->line, 0, 180);
		line = line->next;
	}
	e->count.lgt++;
	new->next = NULL;
	new->cutoff_outer = cos((new->cutoff + new->cutoff_outer) * 0.5 * DEG2RAD);
	new->cutoff = cos(new->cutoff * 0.5 * DEG2RAD);
	return (new);
}

t_lgt			*parse_light(t_env *e, t_line *light_line)
{
	t_line	*line;
	t_lgt	*light;
	t_lgt	*current;

	line = light_line->next->next;
	current = NULL;
	current = create_light(e, line);
	light = current;
	while (line != NULL)
	{
		if (ft_strstr(line->line, "- light:"))
		{
			current->next = create_light(e, line->next);
			current = current->next;
		}
		line = line->next;
	}
	return (light);
}
