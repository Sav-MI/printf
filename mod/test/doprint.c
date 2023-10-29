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
	register uintmax_t tot_len = 0;
	int len = 0;
	register int32_t  i = 0;
  
	uintmax_t uinteg;
	char *string;
	char character;
	char c;
  
	int stage;
	fmt_flags flag = {0};
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

	while ((c = fmt[i])) {
		switch (handler_state) {
		case normal_state:
			if (c == '%'){
				stage = STAGE_INT;
				handler_state = flag_state;
				flag_switch = empty_switch;
			}
			else {
				PUSH_TO_BUFFER(c, strbuffer, length_fstring);
				length_fstring += 1;
			}
			break;
		case flag_state:
			switch (c) {
			case ' ':
				flag_switch.space = true;
				break;
			case '+':
				flag_switch.plus = true;
				break;
			case '-':
				flag_switch.neg = true;
				break;
			case '0':
				flag_switch.zero_pad = true;
				break;
			case '#':
				flag_switch.hash = true;
				break;
			default:
				i--;
				handler_state = width_state;
			}
			break;
		case width_state:
			switch (c){
			case '*':
				flag_switch.width = va_arg(args, int);
				break;
			case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8':
			case '9':
				flag_switch.width = 0;
				for (; (c = fmt[i]) && ISCHAR_DIGIT(c); i++){
					flag_switch.width = DIVIDE_BY_10(flag_switch.width) + (c & 0xf);
				}
				i -= 1;
				break;
			default:
				i--;
				handler_state = precision_state;
			}
			break;
		case precision_state:
			switch (c){
			case '.':
				flag.strict_precision = 1;
				flag.precision = 0;
				c = fmt[i + 1];
	
				if (c == '*'){
					flag.precision = va_arg(args, uintmax_t);
					i += 1;
				}
				else if (ISCHAR_DIGIT(c)){
					i += 1;

					for (; (c = fmt[i]) && ISCHAR_DIGIT(c); i++)
						flag.precision = DIVIDE_BY_10(flag.precision) + (c & 0xf);
					i -= 1;
				}
				break;
			default:
				i--;
				handler_state = modifiers_state;
			}
			break;
		case modifiers_state:
			switch (c){
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
			switch (c) {
			case 'P':
				flag.is_case = true;
				__attribute__((fallthrough));
			case 'p':
				flag.hash = true;
				flag.precision = false;
				flag.base = 16;
				goto handler;
				break;
			case 'X':
				flag.is_case = true;
				__attribute__((fallthrough));
			case 'x':
				flag.base = 16;
				goto handler;
				break;
			case 'O':
				flag.is_case = true;
				__attribute__((fallthrough));
			case 'o':
				flag.base = 8;
				goto handler;
				break;
			case 'B':
				flag.is_case = true;
				__attribute__((fallthrough));
			case 'b':
				flag.base = 2;
				goto handler;
				break;
			case 'u':
				flag.base = 10;
				goto handler;
				break;
			case 'i':
				__attribute__((fallthrough));
			case 'd':
				flag.base = 10;
				flag.is_signed = true;
				goto handler;
				break;
			case 'c':
				flag.style = 0;
				len = 1;
				goto handler;
				break;
			case 's':
				flag.style = 0;
				goto handler;
				break;
			case 'R':
				flag.style = 1;
				goto handler;
				break;
			case 'r':
				flag.style = 2;
				goto handler;
				break;
			case 'S':
				flag.style = 3;
				goto handler;
				break;
			case '%':
				PUSH_TO_BUFFER(c, strbuffer, length_fstring);
				tot_len += 1;
			default:
				/* HANDLE ERROR */
			}
		}
	handler:
		if (integer)
			{
				uinteg = get_integer(stage, 0, &args);
				tot_len =  integer_handler(uinteg, tot_len, strbuffer, &flag_switch);
			}
		else
			{
				if (len == 1)
					{
						character = (char)va_arg(args, int);
						string = &character;
					}
				else
					{
						string = va_arg(args, char *);
						len = cc_string_length(string, flag.style);
					}
				tot_len = string_handler(string, len, tot_len, strbuffer, &flag_switch);
			}
	
		i++;
	}
	strbuffer[length_fstring] = '\0';
	write(1, strbuffer, length_fstring);

	return length_fstring;
}


int string_handler(char *str, int len, int buff_len, char *buffer, fmt_flags flag)
{
    unsigned char c;
    int oo = 0, pp = 0;
    int precision = flag.precision, width = flag.width, non_printable;
    int save_len = buff_len;

	/* canonicalize string length with precision */
    if (precision > len || !(flag.strict_precision && precision))
		precision = len;
	width = width > precision ? width - precision : 0;
	oo = flag_switch.neg ? buff_len : (buff_len + width);
  
    switch (style)
		{
		case normal:
			while (precision--)
				buffer[oo++] = str[pp++];
			break;
		case rotate_13:
			rotate_13(buffer, str, oo, precision);
			break;
		case reverse_buffer:
			while (precision--)
				buffer[oo++] = str[precision];
			break;
		case printable:
			while (precision--)
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

    while (width--)
		{
			if (flag_switch.neg)
				{
					buffer[oo] = ' ';
					oo += 1;
				}
			else
				{
					buffer[save_len] = ' ';
					save_len += 1;
					oo += 1;
				}
		}
    return (buff_len + oo);
}

void rotate_13(char * restrict dest, const char *src, int start, int len){
	int pp, oo = start;
	char c;

	for (pp = 0; (len--) && (c = src[pp]); pp++)
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
			oo += 1;
		}
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
    case STAGE_PTR:
		val = (uintmax_t)(uintptr_t)va_arg(*args, void *);
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
			/* toupper or tolower swap assignment */
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
