/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 12:18:47 by pkangas           #+#    #+#             */
/*   Updated: 2024/01/23 12:23:03 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdarg.h>
#include <unistd.h>

int	get_arg(const char *format, va_list args)
{
	int	count;

	count = 0;
	if (*format == 'c')
		count = print_char(va_arg(args, int));
	else if (*format == 's')
		count = print_str(va_arg(args, char *));
	else if (*format == 'p')
		count = print_pointer(va_arg(args, void *));
	else if (*format == 'd' || *format == 'i')
		count = print_num(va_arg(args, int), count);
	else if (*format == 'u')
		count = print_unsigned_num(va_arg(args, unsigned int), count);
	else if (*format == 'x' || *format == 'X')
		count = print_hexa(va_arg(args, unsigned int), *format, count);
	else if (*format == '\0')
		return (-2);
	else
		count = write(1, format, 1);
	return (count);
}

int	ft_printf(const char *format, ...)
{
	int		count;
	int		sum;
	va_list	args;

	count = 0;
	sum = 0;
	va_start(args, format);
	while (*format != '\0')
	{
		if (*format == '%')
		{
			format++;
			count = get_arg(format, args);
		}
		else if (*format != '%')
			count = write(1, format, 1);
		if (count == -1 || count == -2)
			break ;
		sum = sum + count;
		format++;
	}
	va_end(args);
	if (count == -1)
		return (-1);
	return (sum);
}
