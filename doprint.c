#include "main.h"
#define BUFFER_SIZE 1024

int _printf(const char * format, ...){
  va_list cc_printfgs, copy_cc_printfgs;
  va_start(cc_printfgs, format);
  va_copy(copy_cc_printfgs, cc_printfgs);
  return handle_format_specifier(format, copy_cc_printfgs);
}

int handle_format_specifier (const char *fmt, va_list args){
  register int8_t  i = 0;
  char *strbuffer = malloc(sizeof (char) * BUFFER_SIZE);
  register intmax_t length_fstring = 0;

  int8_t stage;
  uintmax_t uinteg; /* integers */
  long double db_val; /* floats */
  char *sr_val; /* strings */
  int8_t cr_val; /* char */
  char *rev;
  unsigned char temp_fmt;
  int32_t base, is_hash, is_signed, is_case;

  fmt_flags flag_switch = {
    .space=0, .plus=0, .neg=0, .zero_pad=0, .width=-1, .star=0, .hash=0};
     
  //  UNUSED(HASH);

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
    
  while (temp_fmt = fmt[i]) {
    switch (handler_state) {
    case normal_state:
      if (temp_fmt == '%'){
	stage = STAGE_INT;
	handler_state = flag_state;
	is_hash = 0;
	is_case = 0;
	is_signed = 0;
      }
      else {
	PUSH(temp_fmt, strbuffer, length_fstring);
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
	for (temp_fmt = fmt[i]; ischar_digit(temp_fmt); i++){
	  flag_switch.width = (((flag_switch.width << 1) + (flag_switch.width << 3)) + (temp_fmt & 0xf));
	}
	i -= 1;
	break;
      default:
	i--;
	handler_state = modifiers_state;
      }
      break;
    case precision_state:
      switch (temp_fmt){
      case '.':
	flag_switch.precision = 0;
	
	if (fmt[i + 1] == '*'){
	  flag_switch.precision = va_arg(args, uintmax_t);
	  i += 1;
	}
	else if (ischar_digit(fmt[i + 1])){
	  i += 1;
	  
	  for (temp_fmt = fmt[i]; ischar_digit(temp_fmt); i++){
	  flag_switch.precision = (((flag_switch.precision << 1) + (flag_switch.precision << 3)) + (temp_fmt & 0xf));
	  i -= 1;
	  }
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
      switch (temp_fmt) {
      case 'P':
	is_case = 1;
      case 'p':
	flag_switch.hash += 1;
	flag_switch.precision = SIZE_BIT;
	uinteg = (uintmax_t)(uintptr_t) va_arg(args, void *);
	base = hexadecimal;
	break;
      case 'X':
	is_case = 1;
      case 'x':
	flag_switch.precision = -1;
	base = hexadecimal;
	uinteg = get_integer(stage, is_signed, &args);
	break;
      case 'O':
	is_case = 1;
      case 'o':
	flag_switch.precision = -1;
	base = octadecimal;
	uinteg = get_integer(stage, is_signed, &args);
	break;
      case 'B':
	is_case = 1;
      case 'b':
	uinteg = (uintmax_t)va_arg(args, uintmax_t);
	base = binary;
	//len = int_formt(val, len, is_case, base, is_signed, buff, &fg);
	break;
      case 'u':
	uinteg = get_integer(stage, is_signed, &args);
	base = decimal;
	break;
      case 'i':
      case 'd':
	is_signed = 1;
	base = decimal;
	uinteg = get_integer(stage, is_signed, &args);
	//len = int_formt(val, len, is_case, base, is_signed, buff, &fg);
	break;
      case 'r':
	break;
      case 'R':
	break;
      case 'c':
	flag_switch.precision = -1;
	cr_val = (char)va_arg(args, int);
	//len = str_fmt(&cr_val, buff, 1, len, stage, &fg);
	break;
      case 'S':
	sr_val = va_arg(args, char *);
	//uint8_t k = str_len(sr_val);
	//len = str_fmt(sr_val, buff, k, len, stage, &fg);
	break;
      case 's':
	sr_val = va_arg(args, char *);
	//uint8_t k = str_len(sr_val);
	//len = str_fmt(sr_val, buff, k, len, stage, &fg);
	break;
      }
      handler_state = normal_state;
      break;
    default:
      i--;
      handler_state = normal_state;
    }
    i++;
  }
  strbuffer[length_fstring] = '\0';
}

uintmax_t get_integer(int stage, int is_signed, va_list *args){ 
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


int integer_handler(uintmax_t val, int length_fstring, unsigned int is_case, int base, int is_signed, char *buffer, fmt_flags *flag_switch){
  uintmax_t temp_integ;
  char temp_buff[1024];
  uint8_t hexcase;
  int oo = 0;
  char case_char = 0;
  int local_len = 0;
  int initial_len = length_fstring;
  int rank = 1; //TODO add rank
  int space, plus, zero, width, hash, precision, negative;

  precision = flag_switch->precision;

  if (precision){
    precision ^= width;
    width ^= precision;
    precision ^= width;
    flag_switch->neg = 0;
  }
  else
    width = flag_switch->width;
  
  hash = flag_switch->hash;
  negative = 0;
  plus = space = zero = 0;
  
  if (is_signed)
    temp_integ = (uintmax_t)((intmax_t)val < 0 ? ((negative = 1), (intmax_t)(-val)) : (intmax_t)val);
  else {
    switch (rank){
    case 1:
      temp_integ = (unsigned int)val;
      break;
    case 2:
      temp_integ = (unsigned long)val;
      break;
    default:
      temp_integ = val;
    }
  }
  width -= negative ? 1 : 0;
  
  if ((flag_switch->zero_pad || precision) && !(flag_switch->neg))
    zero = 1;

  if (base == DECIMAL){
    plus = flag_switch->plus && !(negative) ? 1 : 0;
    space = flag_switch->space && !(plus) && !(negative) && !(precision) ? 1 : 0;
  }  

  switch (base){
  case HEXADECIMAL:
    case_char = 'x';
    hexcase = is_case ? 55 : 87;
    do {
      temp_buff[local_len ++] = (temp_integ & 0xf) > 9 ? (char)(hexcase + (temp_integ & 0xf)) : 48 + (temp_integ & 0xf);
      temp_integ >>= 4;
    } while (temp_integ);
    break;
  case DECIMAL:
    do {
      temp_buff[local_len ++] = INTEGER_TO_CHAR(temp_integ % 10);
      temp_integ /= 10;
    } while (temp_integ);
    break;
  case OCTADECIMAL:
    do {
      temp_buff[local_len ++] = INTEGER_TO_CHAR(temp_integ & 7);
      temp_integ >>= 3;
    } while (temp_integ);
    if (hash & val)
      temp_buff[local_len ++] = '0';
    break;
  case BINARY:
    case_char = 'b';
    do {
      temp_buff[local_len ++] = INTEGER_TO_CHAR(temp_integ & 1);
      temp_integ >>= 1;
    }  while (temp_integ);
    break;
  }
  if (precision

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

  if (plus)
    width -= 1;
  
  if (space)
    width -= 1;

  /* canonicalizing width with length of temporary buffer */
  width = width > local_len ? width - local_len : 0;

  IF_APUSH_A_ELSE_B(zero, space, temp_buff, buffer, local_len, length_fstring, ' ');
  IF_APUSH_A_ELSE_B(zero, plus, temp_buff, buffer, local_len, length_fstring, '+');
  IF_APUSH_A_ELSE_B(zero, negative, temp_buff, buffer, local_len, length_fstring, '-');

  /* push pre-width padding */

  while (!(flag_switch->neg) && width --> 0)
    temp_buff[local_len ++] = zero ? '0' : ' ';

  while (local_len --> 0)
    buffer[length_fstring ++] = temp_buff[local_len];

  /* push post-width padding */
  while (flag_switch->neg && width --> 0)
      buffer[length_fstring ++] = zero ? '0' : ' ';

  buffer[length_fstring] = '\0';

  printf("%s", buffer); puts("--");
  return length_fstring;
  
}

int main(){
  //.space=0, .plus=0, .neg=0, .zero_pad=0, .width=-1, .star=0, .hash=0
  int x = 123;
  printf("%9.6d", x); puts("--");
  fmt_flags s = {.space=0, .plus=0, .precision=6, .neg=1, .zero_pad=0, .width=9, .star=0, .hash=1};
  integer_handler(x, 0, 0, 10, 0, (char[1024]){'0'}, &s);
}
