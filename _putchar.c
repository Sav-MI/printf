#include "main.h"

/**
 * _putchar - put char
 * @s: char
 * Return: number of character printed
 */
int _putchar(const char s)
{
	return (write(1, &s, 1));
}
