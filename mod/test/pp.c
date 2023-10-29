#include "main.h"
#include <stdio.h>
#include <string.h>

int main(void){
	fmt_flags flag = {.precision=0, .width=9, .strict_precision = 0, .neg = 0, .style = 0};
	char c = 'A';
	char *tmp = "hell\\xo";
	int p = printf("%9c", c);
	putchar('\n');
	int s = string_mod(&c, 1, 0, (char[128]){0}, flag);

	printf("p = %d : s = %d\n", p, s); 
	return (0);
}

/**
 * string_mod - handles printf's string format and write to stdout
 * @str: string
 * @buffer: stores character
 * @flag: - set flags
 * @len: length of @str
 * @buff_len: length of buffer
 * Return: number of characters printed so far
 */

int string_mod(char *str, int len, int buff_len, char *buffer, fmt_flags flag)
{
	int oo = 0;
	int prsn = flag.precision, width = flag.width;
	int save_len = buff_len;

	if (prsn > len || !(flag.strict_precision || prsn))
		prsn = len;
	width = width > prsn ? width - prsn : 0;
	oo = flag.neg ? buff_len : (buff_len + width);

	switch (flag.style)
		{
		case normal:
			oo += copy_src_dest(str, buffer + oo, 0, prsn);
			break;
		case rotate_13:
			oo += rotate_by_13(str, buffer + oo, prsn);
			break;
		case reverse_buffer:
			oo += copy_src_dest(str, buffer + oo, 1, prsn);
			break;
		case printable:
			oo += copy_printable(str, buffer + oo, prsn);
			break;
		}

	while (width--)
	{
		if (flag.neg)
		{
			buffer[oo] = ' ';
			oo += 1;
		}
		else
			buffer[save_len] = ' ';

		save_len += 1;
	}
	puts(buffer);
	return (oo);
}

/**
 * copy_src_dest - copies prsn characters from src to dest.
 * @src: string to be copied.
 * @dest: destination buffer to copy to.
 * @rev: reverse string marker.
 * @prsn: number of characters to copy.
 * Return: nothing
 */
inline int copy_src_dest(char * __restrict src, char *dest, int rev, int prsn)
{
	int oo = 0;

	for (; (prsn--) > 0; oo++)
		dest[oo] = rev ? src[prsn] : src[oo];
	return (oo);
}
/**
n * copy_printable - copies only printable characters and replaces
 * non printable characters with '\x'.
 * @src: string to be copied.
 * @dest: destination buffer to copy to.
 * @prsn: number of characters to copy.
 * Return: nothing
 */

int copy_printable(const char *__restrict__ src, char *dest, int prsn)
{
	char c;
	int oo = 0, pp = 0;

	while ((prsn--) > 0)
	{
		c = src[pp];
		if (c < 32 || c > 126)
		{
			dest[oo] = '\\';
			oo += 1;
			prsn--;
			dest[oo] = 'x';
		}
		else
			dest[oo] = c;

		pp += 1;
		oo += 1;
	}
	return (oo);
}

/**
 * rotate_13 - rot-13 encoding
 * @src: string to be copied.
 * @dest: destination buffer to copy to.
 * @prsn: number of characters to copy.
 * Return: nothing
 */
int rotate_by_13(const char *__restrict__ src, char *dest, int prsn)
{
	int oo = 0;
	char c;

	for (oo = 0; (prsn--) && (c = src[oo]); oo++)
	{
		if (c > 64 && c < 92)
		{
			if ((c - 64) < 14)
				c += 13;
			else
				c -= 13;
		}
		else if (c > 96 && c < 123)
		{
			if ((c - 96) < 14)
				c += 13;
			else
				c -= 13;
		}
		dest[oo] = c;
	}
	return (oo);
}
