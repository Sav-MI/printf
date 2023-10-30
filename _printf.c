#include "main.h"

/**
 * _printf - printf
 * @format: format string
 * Return: number of characters printed
 */
int _printf(const char *format, ...)
{
	int i = 0, n __attribute__((unused)) = 0;
	char c;
	va_list args;

	if (format == NULL || (*format == '%' && format[1] == '\0'))
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
				n += printf_string(args);
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

/**
  * printf_string - print a string.
  * @val: argumen t.
  * Return: the length of the string.
  */

int printf_string(va_list val)
{
	char *s;
	int i, len;

	s = va_arg(val, char *);
	if (s == NULL)
	{
		s = "(null)";
		len = _strlen(s);
		for (i = 0; i < len; i++)
			_putchar(s[i]);
		return (len);
	}
	else
	{
		len = _strlen(s);
		for (i = 0; i < len; i++)
			_putchar(s[i]);
		return (len);
	}
}

/**
 * _strlen - string length
 * @s: string
 * Return: length of string
 */
int _strlen(char *s)
{
	int oo = 0;

	while (s[oo])
		oo += 1;
	return (oo);
}
