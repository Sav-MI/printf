int handle_format_specifier (const char *fmt, va_list args){
	char *strbuffer = malloc(sizeof (char) * BUFFER_SIZE);
	register int32_t  i = 0, stage = 0, len = 0, len;
  
	uintmax_t uinteg;
	char c; /* temporary variable that stores any character */
  
	fmt_flags flag = {0}, empty_switch = flag;

	if (fmt == NULL || fmt[0] == '\0')
		return 0;

	while ((c = fmt[i]))
		{
			switch (state)
				{
				case normal_state:
					__normal_state_macro(c, stage, state, strbuff, flag, len);
					break;
				case flag_state:
					__flag_state_macro(c, i, flag, state);
					break;
				case width_state:
					__width_state_macro(c, fmt, i, flag, state);
					break;
				case precision_state:
					__precision_state_macro(c, fmt, i, flag, state);
					break;
				case modifiers_state:
					__modifier_state_macro();
					break;
				case output_mod_state:
					__output_modifier_macro(c, len, flag, state, handler, strbuff);
					break;
				}
			i++;
		}
	return (tot_len);
}

int handler(int type, int len, fmt_flags flag){ 
	const char *string;
	char c;

	if (type)
	{
		uinteg = get_integer(stage, flag.is_sgned, &args);
		len = integer_handler(uinteg, len, buffer, flag);
	}
	else
	{
		if (len == 1)
		{
			c = (char)va_arg(args, int);
			string = &c;
		}
		else
		{
			if ((string = va_arg(args, char *)) == NULL)
			{
				string = "(null)";
				flag.style = 0;
				len = 6;
			}
			else
				len = cc_string_length(string, flag.style);
		}
		len = string_handler(string, len, buffer, &flag);
	}
}

char *cmalloc (size_t size)
{
	char *value = malloc (size);

	if (value == 0)
		fatal ("virtual memory exhausted");
	return value;
}
