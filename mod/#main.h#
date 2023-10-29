#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include <errno.h>

#define inline __##inline##__
#define restrict __restrict__
#define ___res_char char *__restrict__
typedef uint32_t __u32_t;

typedef struct {
	int width;
	int precision;
	int style;
	bool is_sgned;
	bool is_case;
	bool is_char;
	bool space;
	bool plus;
	bool neg;
	bool zero_pad;
	bool strict_precision;
	bool star;
	bool hash;
	unsigned char base; 
} fmt_flags;

typedef enum {
	normal_state,
	flag_state,
	width_state,
	precision_state,
	modifiers_state,
	output_mod_state
} handler_state;

/** @style_state: styles to print buffer */
enum style_state
	{
		normal,
		rotate_13,
		reverse_buffer,
		printable
	};

enum stage{
  STAGE_CHAR = -2,
  STAGE_SHORT = -1,
  STAGE_INT = 0,
  STAGE_LONG = 1,
  STAGE_PTR = 2
};

typedef enum base_type {
  decimal = 10,
  hexadecimal = 16,
  octadecimal = 8,
  binary = 2
} base_type;

#define HEXADECIMAL 16
#define DECIMAL 10
#define OCTADECIMAL 8
#define BINARY 2

#define MIN_STAGE STAGE_CHAR
#define MAX_STAGE STAGE_LONG
#define BUFFER_SIZE 1024

#define SIZE_BIT (3 + sizeof (void *) * (CHAR_BIT >> 2));

#define UNUSED(c) (void)(c)

#define  PUSH_TO_BUFFER(OBJ, BUFFER, LEN)\
  (*(BUFFER + LEN) = OBJ)

#define ISCHAR_DIGIT(STR_VAL)			\
  (STR_VAL >= 48 && STR_VAL <= 57)

#define IF_APUSH_A_ELSE_B(CASE, R_CASE, A, B, LEN_A, LEN_B, OBJ)\
  do {								\
    if (!CASE){							\
      if (R_CASE)						\
	A[LEN_A ++] = OBJ;					\
    }								\
    else {							\
      if (R_CASE)						\
	B[LEN_B ++] = OBJ;					\
    }								\
  } while (0)
#define CLEAN_UP_PRECISION(PRSN, BUFFER, LEN)\
  do {\
  PRSN = PRSN > LEN ? PRSN - LEN : 0;\
  while (PRSN --> 0)\
    BUFFER[LEN ++] = '0';\
  } while (0)

#define INTEGER_TO_CHAR(INTEGER) ((unsigned char)((INTEGER) + 48))
#define DIVIDE_BY_10(INTEGER)\
  ((INTEGER << 1) + (INTEGER << 3))

#define __is_upper(c) ((c) > 64 && (c) < 92)
#define __is_lower(c) ((c) > 96 && (c) < 123)
#define __is_digit(c) ((c) > 47 && (c) < 58)
#define __is_non_printable(c) (c < 32 || (c) > 126)
#define __div_10_macro(n)(((n) << 1) + ((n) << 3))
#define _push_to_buffer(b, c, i)\
	do { (*((b) + (i)) = (c)); i++; } while (0)
#define __strtoi_macro(s, c, n, i)					\
	do { for (; ((c) = s[i]) && __is_digit(c); i++)			\
			n = __div_10_macro(n) + ((c) & 0xf); } while (0)
#define __string_copy_macro(src, dest, rev, prsn, oo)\
	do {register int oo = 0; for (; (prsn--) > 0; oo++) dest[oo] = rev ? src[prsn] : src[oo]; } while (0)


int _printf(const char *, ...);
int handle_format_specifier (const char *, va_list);
uintmax_t get_integer(int, int, va_list *);
int integer_handler(uintmax_t, int, unsigned int, int, int, char *, fmt_flags *);
int string_length(const char *__restrict__, int);
int string_mod(char *, int, fmt_flags);
uint32_t copy_printable(const char *___restrict__, char *__restrict__, int);
uint32_t rotate_by_13(const char *__restrict__, char *__restrict__, size_t);
int _putchar(const char);
int __puts(const char *__restrict__, int);
#endif
