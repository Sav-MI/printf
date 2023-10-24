#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
typedef struct fmt_flags fmt_flags;

typedef struct fmt_flags {
  int space;
  int plus;
  int neg;
  int zero_pad;
  int width;
  int precision;
  int star;
  int hash;
} flag;

enum stage{
  STAGE_CHAR = -2,
  STAGE_SHORT = -1,
  STAGE_INT = 0,
  STAGE_LONG = 1
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

#define SIZE_BIT (3 + sizeof(void *) * (CHAR_BIT) / 4)
#define reset(val) val = 0
#define isnum_min 48
#define isnum_max 57
#define UNUSED(c) (void)(c)
#define ADD_1(x) x += 1
#define SUB_1(x) x -= 1
#define  PUSH(x, buff, t)\
  (*(buff + t) = x)
#define ischar_digit(str_val)\
  (str_val >= isnum_min && str_val <= isnum_max)
#define SWITCH_OFF(val) val = 0

#define PUSH_BASE_TAG(BUFFER, TAG_UPPER, TAG_LOWER, CASE, LEN)\
  do {\
  BUFFER[LEN ++] = CASE ? TAG_UPPER : TAG_LOWER;	\
  BUFFER[LEN ++] = '0';\
  } while (0)
#define IF_APUSH_A_ELSE_B(CASE, R_CASE, A, B, LEN_A, LEN_B, OBJ)\
  if (!CASE){							\
    if (R_CASE)							\
      A[LEN_A ++] = OBJ;					\
  }								\
  else {							\
    if (R_CASE)							\
      B[LEN_B ++] = OBJ;					\
  }								\

#define INTEGER_TO_CHAR(INT) (unsigned char)((INT) + 48)

#define HEX_F 70
#define HEX_f 102
#define UPPER_A 65
#define UPPER_Z 90
#define LOWER_a 97
#define LOWER_z 122

/* template for array reversal */
#define REV_AR(typename, val) \
  typename *REV_AR_##val(typename *oop, int len){     \
  int haf_len = (len / 2); \
  int i = 0; \
  --len; \
  while (i != haf_len){ \
  oop[i] ^= oop[len - i]; \
  oop[len - i] ^= oop[i]; \
  oop[i] ^= oop[len - i]; \
  i++;   \
  }           \
  return oop;  \
  }                \

int _printf(const char *, ...);
int handle_format_specifier (const char *, va_list);
uintmax_t get_integer(int, int, va_list *);
int integer_handler(uintmax_t, int, unsigned int, int, int, char *, fmt_flags *);
#endif
