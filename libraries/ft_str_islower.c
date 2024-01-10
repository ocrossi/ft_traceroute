/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_str_islower.c                                 .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ocrossi <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/24 12:13:32 by ocrossi      #+#   ##    ##    #+#       */
/*   Updated: 2018/10/24 12:16:14 by ocrossi     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

int		ft_str_islower(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] < 97 && str[i] > 122)
			return (0);
		i++;
	}
	return (1);
}