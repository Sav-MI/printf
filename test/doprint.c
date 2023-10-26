#include "main.h"
#define BUFFER_SIZE 1024

int _printf(const char * format, ...){
  va_list cc_printfgs, copy_cc_printfgs;
  va_start(cc_printfgs, format);
  va_copy(copy_cc_printfgs, cc_printfgs);
  return handle_format_specifier(format, copy_cc_printfgs);
}

int handle_format_specifier (const char *fmt, va_list args){
  char *strbuffer = malloc(sizeof (char) * BUFFER_SIZE);
  register intmax_t length_fstring = 0;
  register int32_t  i = 0;
  
  uintmax_t uinteg;
  char *string_temporary_val;
  char char_temporary_val;
  unsigned char temp_fmt;
  
  int is_case, stage, strict_precision = 0;
  fmt_flags flag_switch = {0, 0, 0, 0, 0, 0, 0, 0};
  fmt_flags empty_switch = flag_switch;

  enum {
    normal_state,
    flag_state,
    width_state,
    precision_state,
    modifiers_state,
    output_mod_state
  } handler_state;

  handler_state = normal_state;
  stage = 0;

  while ((temp_fmt = fmt[i])) {
    switch (handler_state) {
    case normal_state:
      if (temp_fmt == '%'){
	stage = STAGE_INT;
	handler_state = flag_state;
	is_case = 0;
	flag_switch = empty_switch;
      }
      else {
	PUSH_TO_BUFFER(temp_fmt, strbuffer, length_fstring);
	length_fstring += 1;
      }
      break;
    case flag_state:
      switch (temp_fmt) {
      case ' ':
	flag_switch.space = 1;
	break;
      case '+':
	flag_switch.plus = 1;
	break;
      case '-':
	flag_switch.neg = 1;
	break;
      case '0':
	flag_switch.zero_pad = 1;
	break;
      case '#':
	flag_switch.hash = 1;
	break;
      default:
	i--;
	handler_state = width_state;
      }
      break;
    case width_state:
      switch (temp_fmt){
      case '*':
	flag_switch.width = va_arg(args, int);
	break;
      case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8':
      case '9':
	flag_switch.width = 0;
	for (; (temp_fmt = fmt[i]) && ISCHAR_DIGIT(temp_fmt); i++){
	  flag_switch.width = DIVIDE_BY_10(flag_switch.width) + (temp_fmt & 0xf);
	}
	i -= 1;
	break;
      default:
	i--;
	handler_state = precision_state;
      }
      break;
    case precision_state:
      switch (temp_fmt){
      case '.':
	strict_precision = 1;
	flag_switch.precision = 0;
	temp_fmt = fmt[i + 1];
	
	if (temp_fmt == '*'){
	  flag_switch.precision = va_arg(args, uintmax_t);
	  i += 1;
	}
	else if (ISCHAR_DIGIT(temp_fmt)){
	  i += 1;

	  for (; (temp_fmt = fmt[i]) && ISCHAR_DIGIT(temp_fmt); i++)
	    flag_switch.precision = DIVIDE_BY_10(flag_switch.precision) + (temp_fmt & 0xf);
	  i -= 1;
	}
	break;
      default:
	i--;
	handler_state = modifiers_state;
      }
      break;
    case modifiers_state:
      switch (temp_fmt){
      case 'h':
	stage -= 1;
	break;
      case 'l':
	stage += 1;
	break;
      default:
	i--;
	handler_state = output_mod_state;
	stage = stage < MIN_STAGE ? MIN_STAGE : stage;
	stage = stage > MAX_STAGE ? MAX_STAGE : stage;
      }
      break;
    case output_mod_state:
      handler_state = normal_state;
      switch (temp_fmt) {
      case 'P':
	is_case = 1;
	__attribute__((fallthrough));
      case 'p':
	flag_switch.hash = 1;
	flag_switch.precision = 0;
	uinteg = (uintmax_t)(uintptr_t)va_arg(args, void *);
	length_fstring = integer_handler(uinteg, length_fstring, is_case, 16, 0, strbuffer, &flag_switch);
	break;
      case 'X':
	is_case = 1;
	__attribute__((fallthrough));
      case 'x':
	uinteg = get_integer(stage, 0, &args);
	length_fstring = integer_handler(uinteg, length_fstring, is_case, 16, 0, strbuffer, &flag_switch);
	break;
      case 'O':
	is_case = 1;
	__attribute__((fallthrough));
      case 'o':
	uinteg = get_integer(stage, 0, &args);
	length_fstring = integer_handler(uinteg, length_fstring, is_case, 8, 0, strbuffer, &flag_switch);
	break;
      case 'B':
	is_case = 1;
	__attribute__((fallthrough));
      case 'b':
	uinteg = get_integer(stage, 0, &args);
	length_fstring = integer_handler(uinteg, length_fstring, is_case, 2, 0, strbuffer, &flag_switch);
	break;
      case 'u':
	uinteg = get_integer(stage, 0, &args);
	length_fstring = integer_handler(uinteg, length_fstring, 0, 10, 0, strbuffer, &flag_switch);
	break;
      case 'i':
	__attribute__((fallthrough));
      case 'd':
	uinteg = get_integer(stage, 1, &args);
	length_fstring = integer_handler(uinteg, length_fstring, 0, 10, 1, strbuffer, &flag_switch);
	break;
      case 'c':
	char_temporary_val = (unsigned char)va_arg(args, int);
	length_fstring = string_handler(&char_temporary_val, 0, strict_precision, length_fstring, strbuffer, &flag_switch);
	break;
      case 's':
	string_temporary_val = va_arg(args, char *);
	length_fstring = string_handler(string_temporary_val, 0, strict_precision, length_fstring, strbuffer, &flag_switch);
	break;
      case 'R':
	string_temporary_val = va_arg(args, char *);
	length_fstring = string_handler(string_temporary_val, 1, strict_precision, length_fstring, strbuffer, &flag_switch);
	break;
      case 'r':
	string_temporary_val = va_arg(args, char *);
	length_fstring = string_handler(string_temporary_val, 2, strict_precision, length_fstring, strbuffer, &flag_switch);
	break;
      case 'S':
	string_temporary_val = va_arg(args, char *);
	length_fstring = string_handler(string_temporary_val, 3, strict_precision, length_fstring, strbuffer, &flag_switch);
        break;
      case '%':
	PUSH_TO_BUFFER(temp_fmt, strbuffer, length_fstring);
	length_fstring += 1;
      default:
	/* HANDLE ERROR */
      }
    }
      i++;
  }
  strbuffer[length_fstring] = '\0';
  write(1, strbuffer, length_fstring);

  return length_fstring;
}


int string_handler(char *str, int style, int strict_precision, int length_fstring, char *buffer, fmt_flags *flag_switch)
{
    int len = 0;
    unsigned char c;
    int oo = 0, pp = 0;
    int precision, width, non_printable;
    int save_len __attribute__((unused));
    
    enum style_state
	{
	    normal,
	    rotate_13,
	    reverse_buffer,
	    printable
	};
  
    while ((c = str[len]))
	{
	    if ((style == printable) && (c < 32 || c > 126))
		non_printable += 1;
	    len += 1;
	}
    len += non_printable;
  
    precision = flag_switch->precision;

    /* canonicalize string length with precision */
    if (precision > len || !(strict_precision && precision))
	precision = len;

    /* width length */
    width = flag_switch->width;
    width = width > precision ? width - precision : 0;

    if (flag_switch->neg)
	oo = length_fstring;
    else
	{
	    oo = length_fstring + width;
	    save_len = length_fstring;
	}
  
    switch (style)
	{
	case normal:
	    while (precision --> 0)
		buffer[oo++] = str[pp++];
	    break;
	case rotate_13:
	    for (pp = 0; (precision --> 0) && (c = str[pp]); pp++)
		{
		    if (c > 64 && c < 92){
			if ((c - 64) < 14)
			    c += 13;
			else
			    c -= 13;
		    }
		    else if (c > 96 && c < 123){
			if ((c - 96) < 14)
			    c += 13;
			else
			    c -= 13;
		    }
		    buffer[oo++] = c;
		}
	    break;
	case reverse_buffer:
	    while (precision --> 0)
		buffer[oo++] = str[precision];
	    break;
	case printable:
	    while (precision --> 0)
		{
		    c = str[pp++];
		    if (c < 32 || c > 126)
			{
			    buffer[oo++] = '\\';
			    precision--;
			    buffer[oo++] = 'x';
			}
		    else
			buffer[oo++] = c;
		}
	    break;
	}

    while (width --> 0)
	{
	    if (flag_switch->neg)
		buffer[oo++] = ' ';
	    else
		{
		    buffer[save_len++] = ' ';
		    oo += 1;
		}
	}
    return (length_fstring + oo);
}

int main(){
  int x = 0;
  printf("%12.8s", "he\\xll\\xo"); puts("--");
  _printf("%12.8s", "he\\xll\\xo"); puts("--");
}

uintmax_t get_integer(int stage, int is_signed, va_list *args)
{
    uintmax_t val;
    switch (stage){
    case STAGE_CHAR:
	if (is_signed)
	    val = (uintmax_t)(intmax_t)(char)va_arg(*args, int);
	else
	    val = (unsigned char)va_arg(*args, unsigned int);
	break;
    case STAGE_SHORT:
	if (is_signed)
	    val = (uintmax_t)(intmax_t)(short)va_arg(*args, int);
	else
	  val = (uintmax_t)(unsigned short)va_arg(*args, unsigned int);
	break;
    case STAGE_INT:
	if (is_signed)
	    val = (uintmax_t)(intmax_t)va_arg(*args, int);
	else
	    val = (uintmax_t)va_arg(*args, unsigned int);
	break;
    case STAGE_LONG:
	if (is_signed)
	    val = (uintmax_t)(intmax_t)va_arg(*args, long);
	else
	  val = (uintmax_t)va_arg(*args, unsigned long);
	break;
    default:
	break;
    }
    return val;
}


int integer_handler(uintmax_t val, int length_fstring, unsigned int is_case, int base, int is_signed, char *buffer, fmt_flags *flag_switch)
{
  uintmax_t temp_integ;
  char temp_buff[1024];
  uint8_t hexcase;
  char case_char = 0;
  int local_len = 0;
  int space, plus, zero, width, hash, precision, negative;
  
  width = flag_switch->width;
  precision = flag_switch->precision;
  hash = flag_switch->hash;
  negative = 0;
  plus = space = zero = 0;

  /* extract negative sign */
  if (is_signed && ((intmax_t)val < 0)){
    temp_integ = -(intmax_t)(val);
    negative = 1;
  }
  else
    temp_integ = val;
      
  width -= negative ? 1 : 0;

  if (flag_switch->zero_pad && !(flag_switch->neg))
    zero = 1;

  if (base == DECIMAL){
    plus = flag_switch->plus && !(negative) ? 1 : 0;
    space = flag_switch->space && !(plus) && !(negative) ? 1 : 0;
  }

  if (plus)
    width -= 1;

  if (space)
    width -= 1;

  if (precision == false && temp_integ == false)
    goto skip_integer_handling;
  
  switch (base){
  case HEXADECIMAL:
    case_char = 'x';
    hexcase = is_case ? 55 : 87;
    do {
      temp_buff[local_len ++] = (temp_integ & 0xf) > 9 ? (char)(hexcase + (temp_integ & 0xf)) : 48 + (temp_integ & 0xf);
      temp_integ >>= 4;
    } while (temp_integ);
    CLEAN_UP_PRECISION(precision, temp_buff, local_len);
    break;
  case DECIMAL:
    do {
      temp_buff[local_len ++] = INTEGER_TO_CHAR(temp_integ % 10);
      temp_integ /= 10;
    } while (temp_integ);
    CLEAN_UP_PRECISION(precision, temp_buff, local_len);
    break;
  case OCTADECIMAL:
    do {
      temp_buff[local_len ++] = INTEGER_TO_CHAR(temp_integ & 7);
      temp_integ >>= 3;
    } while (temp_integ);
    if (hash && val)
      temp_buff[local_len ++] = '0';
    CLEAN_UP_PRECISION(precision, temp_buff, local_len);
    break;
  case BINARY:
    case_char = 'b';
    do {
      temp_buff[local_len ++] = INTEGER_TO_CHAR(temp_integ & 1);
      temp_integ >>= 1;
    }  while (temp_integ);
    CLEAN_UP_PRECISION(precision, temp_buff, local_len);
    break;
  }
 skip_integer_handling: /* just to skip the above code */

  /* push the base character */

  if (base != DECIMAL && base != OCTADECIMAL){
    if (!zero && val && hash){
      temp_buff[local_len ++] = (case_char ^ (is_case << 5));
      temp_buff[local_len ++] = '0';
    }
    else if (zero && val && hash) {
      buffer[length_fstring ++] = '0';
      buffer[length_fstring ++] = case_char ^ (is_case << 5);
      width -= 2;
    }
  }

  /* canonicalizing width with length of temporary buffer */
  width = width > local_len ? width - local_len : 0;

  IF_APUSH_A_ELSE_B(zero, space, temp_buff, buffer, local_len, length_fstring, ' ');
  IF_APUSH_A_ELSE_B(zero, plus, temp_buff, buffer, local_len, length_fstring, '+');
  IF_APUSH_A_ELSE_B(zero, negative, temp_buff, buffer, local_len, length_fstring, '-');

  //* push pre-width padding */

  while (!(flag_switch->neg) && width --> 0)
    temp_buff[local_len ++] = zero ? '0' : ' ';

  while (local_len --> 0)
    buffer[length_fstring ++] = temp_buff[local_len];

  /* push post-width padding */
  while (flag_switch->neg && width --> 0)
      buffer[length_fstring ++] = zero ? '0' : ' ';

  return length_fstring;

}
