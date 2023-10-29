
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

#define __write_to_buffer(condition, buffer)
