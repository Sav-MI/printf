#ifndef MAIN_H
#define MAIN_H
#include <stdarg.h>
#include <unistd.h>
#include <stddef.h>
int _printf(const char *format, ...);
int _putchar(const char);
int printf_string(va_list val);
int _strlen(char *s);
#endif
