#include "main.h"

/**
 * _printf - printf
 * @format: format string
 * Return: number of characters printed
 */
int _printf(const char *format, ...)
{
	int i = 0, oo = 0, n = 0;
	char c, *cc;
	va_list args;

	va_start(args, format);

	if (format == (char *)0)
		return (-1);

	while ((c = format[i]) != '\0')
	{
		if (c == '%')
		{
			c = format[++i];
			switch (c)
			{
			case 'c':
				n += _putchar((char)va_arg(args, int));
				break;
			case 's':
				cc = va_arg(args, char *);

				if (cc == NULL)
					cc = "(null)";

				while ((c = cc[oo++]) != '\0')
					n += _putchar(c);
				break;
			case '%':
				n += _putchar('%');
				break;
			default:
				return (0);
			}
		}
		else
			n += _putchar(c);
		i++;
	}
	return (n);
}

/**
 * _putchar - put char
 * @s: char
 * Return: number of character printed
 */
int _putchar(const char s)
{
	return (write(1, &s, 1));
}
