/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pow.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/20 06:58:50 by tbeauman          #+#    #+#             */
/*   Updated: 2016/03/20 06:59:29 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

double	ft_pow(double n, int pow)
{
	while (pow--)
		n *= n;
	return (n);
}