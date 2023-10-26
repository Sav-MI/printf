int string_handler(char *str, int style, int strict_prsn, int len_str, char *buffer, fmt_flags *flags)
{
    unsigned char c;
    int len = 0;
    int oo = 0, pp = 0;
    int precision, width, non_printable;
    int save_len __attribute__((unused));

    enum style_state {
	normal,
	rotate_13,
	reverse_buffer,
	printable
    };

    while ((c = str[len])){
	if ((style == printable) && (c < 32 || c > 126))
	    non_printable += 1;
	len += 1;
    }
    len += non_printable;

    precision = flags->precision;

    /* canonicalize string length with precision */
    if (precision > len || !(strict_prsn && precision))
	precision = len;

    /* width length */
    width = flags->width;
    width = width > precision ? width - precision : 0;

    if (flags->neg)
	oo = len_str;
    else {
	oo = len_str + width;
	save_len = len_str;
    }

    switch (style){
    case normal:
	while (precision --> 0)
	    buffer[oo ++] = str[pp ++];
	break;
    case rotate_13:
	for (pp = 0; (precision --> 0) && (c = str[pp]); pp++){
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
	    buffer[oo ++] = c;
	}
	break;
    case reverse_buffer:
	while (precision --> 0)
	    buffer[oo ++] = str[precision];
	break;
    case printable:
	while (precision --> 0){
	    c = str[pp ++];
	    if (c < 32 || c > 126){
		buffer[oo ++] = '\\';
		precision--;
		buffer[oo ++] = 'x';
	    }
	    else
		buffer[oo ++] = c;
	}
	break;
    }

    while (width --> 0){
	if (flags->neg)
	    buffer[oo ++] = ' ';
	else {
	    buffer[save_len ++] = ' ';
	    oo += 1;
	}
    }
    return len_str + oo;
}
