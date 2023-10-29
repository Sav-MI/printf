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

	va_start(args, str);

	if (str == NULL || str[0] == '\0')
		return (0);

	while ((c = str[i]) != '\0')
	{
		if (c == '%')
		{
			c = str[++i];
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
				n += _putchar('%');
			}
		}
		else
			n += _putchar(c);
		i++;
	}
	return (n);
}

#include <stdio.h>
int main(void)
{
	printf("%d\n", _printf("hello"));

	return (0);
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
