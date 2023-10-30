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

	if (format == (char *)0)
		return (-1);
	va_start(args, format);
	while ((c = format[i]) != '\0')
	{
		if (c == '%')
		{
			c = format[++i];
			switch (c)
			{
			case 'c':
				n += _putchar((unsigned char)va_arg(args, int));
				break;
			case 's':
				cc = va_arg(args, char *);
				if (cc == (char *)0)
					cc = "(null)";

				while ((c = cc[oo++]) != '\0')
					n += _putchar(c);
				break;
			case '%':
				n += _putchar('%');
				break;
			case '\0':
				return (-1);
			default:
				n += _putchar('%');
				i--;
			}
		}
		else
			n += _putchar(c);
		i++;
	}
	return (n);
}
