/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_arg_ptr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scollon <scollon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/19 11:51:14 by scollon           #+#    #+#             */
/*   Updated: 2016/03/06 10:14:28 by scollon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <printf_utils.h>

void		print_arg_ptr(t_e *e, t_a *arg, int *i)
{
	*i = print_mem(va_arg(e->ap, void *), arg);
}
