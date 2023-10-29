#include "main.h"

/**
 * _putchar - prints character
 * @__c: character
 * Return: return value of write().
 */
inline int _putchar(const char __c)
{
	return (write(1, &__c, 1));
}

/**
 * __puts - prints character
 * @__s: character.
 * @prsn: number of character to print.
 * Return: return value of write().
 */
inline int __puts(const ___res_char __s, int prsn)
{
	return (write(1, __s, prsn));
}

/**
 * string_mod - handles printf's string format and write to stdout
 * @str: string
 * @flag: - formatting flags.
 * @len: length of @str.
 * Return: number of characters printed so far
 */

int string_mod(char *str, int len, fmt_flags flag)
{
	register uint32_t oo = 0, cc __attribute__((unused)) = 0;
	int prsn = flag.precision, width = flag.width;
	char *buffer = malloc(sizeof(char) * (len + width + prsn));
	char *tmp = str;

	if (prsn > len || !(flag.strict_precision || prsn))
		prsn = len;
	width = width > prsn ? width - prsn : 0;

	/* front padding for trailing whitespace */
	if (!(flag.neg))
		while (width--)
			_push_to_buffer(buffer, ' ', oo);

	switch (flag.style)
		{
		case rotate_13:
			oo += rotate_by_13(str, buffer + oo, prsn);
			goto push_char;
		case reverse_buffer:
			__string_copy_macro(str, buffer, 1, cc, oo);
			goto push_char;
		case printable:
			oo += copy_printable(str, buffer + oo, prsn);
		}
	push_char:

	/* whitespace back padding */
	if (flag.neg)
		while (width--)
			_push_to_buffer(buffer, ' ', oo);

	__puts(buffer, oo);
	free(buffer);
	return (oo);
}
#include <stdio.h>
#include <string.h>
int main(void){
	fmt_flags flag = {.width=5, .style=1, .neg = 1, .precision = 3};
	char *x = "hello";
	int p = printf("%-5.3s", x); putchar('0');
	putchar('\n');
	int c = string_mod(x, strlen(x), flag); putchar('0');
	putchar('\n');
	printf("p - %d c - %d\n", p, c);
}
/**
 * copy_printable - copies only printable characters and replaces
 * non printable characters with '\x'.
 * @src: string to be copied.
 * @dest: destination buffer to copy to.
 * @prsn: number of characters to copy.
 * Return: nothing
 */

inline int to_hex(char *s, unsigned long num, bool _case)
{
	char c __attribute__((unused)) = __case ? 55 : 87;
	register unsigned long oo = 0, aa = 0, pp = num;

	while ((pp >>= 4) && (oo += 1));
	aa = oo;
	while ((oo--) > 0)
	{
		pp = num & 0xf;
		s[oo] = pp > 9 ? (c + pp) : (48 + pp);
		num >>= 4;
	}
	return aa;
}

inline uint32_t copy_printable(const ___res_char src, ___res_char dest, int prsn)
{
	char c;
	register uint32_t oo = 0;

	for (; (c = src[oo]) && (prsn--) > 0; oo++)
	{
		if (__is_non_printable(c))
		{
			dest[oo] = '\\';
			dest[oo + 1] = 'x';
			prsn -= to_hex(dest, c, true);
			oo += 3;
		}
		else
			_putchar(c);

		oo += 1;
	}
	return (oo);
}

/**
 * rotate_by_13 - rot-13 encoding
 * @src: sampling string.
 * @dest: destination buffer to copy to.
 * @prsn: number of characters to copy.
 * Return: nothing
 */

extern inline uint32_t rotate_by_13(const ___res_char src, ___res_char dest, size_t prsn)
{
	register uint32_t oo = 0, tmp = 0;
	char c;

	for (oo = 0; (prsn--) > 0 && (c = src[oo]); oo++)
	{
		tmp = __is_upper(c) ? 64 : __is_lower(c) ? 96 : 0;
		if (tmp)
		{
			if ((c - tmp) < 14)
				c += 13;
			else
				c -= 13;
		}
		dest[oo] = c;
	}
	return (oo);
}
