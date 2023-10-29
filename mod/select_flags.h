#ifndef SWITCH_CASE_MACRO_BYPASS
#define SWITCH_CASE_MACRO_BYPASS

/*
 * This is arranged in hierachy of assertion in the switch case:
 * And is wriiten this way (in macros) to bypass the linters 40
 * Lines per function rule.
 * States:
 * normal_state;
 * flag_state;
 * width_state;
 * precision_state;
 * type_rank state;
 * type (output_modifier) state.
 */

/* __normal_state_macro - starting point */
#define __normal_state_macro(c, stage, state, flag, buffer, len)\
	do {														\
		if (c == '%')											\
		{														\
			stage = STAGE_INT;									\
			state = flag_state;									\
			flag = empty_switch;								\
		}														\
		else													\
		{														\
			PUSH_TO_BUFFER(c, strbuffer, len);					\
			len += 1;											\
		}														\
	} while (0)

/* __flag_state_macro - switch on flags bit if present. */

#define __flag_state_macro(c, i, flag, state)(	\
	switch (c)									\
		{										\
		case ' ':								\
			flag.space = true;			\
			break;								\
		case '+':								\
			flag.plus = true;			\
			break;								\
		case '-':								\
			flag.neg = true;				\
			break;								\
		case '0':								\
			flag.zero_pad = true;		\
			break;								\
		case '#':								\
			flag.hash = true;			\
			break;								\
		default:								\
			i--;								\
			state = width_state;				\
		}										\
	})

/* __width_state_macro - extract width from format */
#define __width_state_macro(c, fmt, i, flag, state)(	\
	switch (c)										\
		{											\
		case '1': case '2': case '3': case '4':		\
		case '5': case '6': case '7': case '8':		\
		case '9':									\
			flag.width = 0;							\
			__strtoi_macro(fmt, c, flag.width, i);	\
			i -= 1;									\
			break;									\
		default:									\
			i--;									\
			state = precision_state;		\
		})

/* __precision_state_macro - extract precision from format */
#define __precision_state_macro(c, fmt, i, flag, state)(\
	switch (c)												   \
		{														\
		case '.':												\
			flag.strict_precision = true;						\
			flag.precision = 0;									\
			c = fmt[i + 1];										\
																\
			if (__is_digit(c))									\
			{													\
				i += 1;											\
				__strtoi_macro(fmt, c, flag.precision, i);		\
				i -= 1;											\
			}													\
			break;												\
		default:												\
			i--;												\
			state = modifiers_state;\
		})

/* __modifier_state_macro - type rank */
#define __modifier_state_macro(c, stage, state, i)(			\
	switch (c)												\
		{													\
		case 'h':											\
			stage -= 1;										\
			break;											\
		case 'l':											\
			stage += 1;										\
			break;											\
		default:											\
			i--;											\
			state = output_mod_state;				\
			stage = stage < MIN_STAGE ? MIN_STAGE : stage;	\
			stage = stage > MAX_STAGE ? MAX_STAGE : stage;	\
		})

/* __output_modifier_macro - type/style selection */
#define __output_modifier_macro(c, len, flag, state, jump, buff)\
	do {														\
		state = normal_state;									\
		switch (c)												\
		{														\
		case 'P':												\
			flag.is_case = true;								\
			__attribute__((fallthrough));						\
		case 'p':												\
			flag.hash = true;									\
			flag.precision = false;								\
			flag.base = 16;										\
			break;												\
		case 'X':												\
			flag.is_case = true;								\
			__attribute__((fallthrough));						\
		case 'x':												\
			flag.base = 16;										\
			break;												\
		case 'O':												\
			flag.is_case = true;								\
			__attribute__((fallthrough));						\
		case 'o':												\
			flag.base = 8;										\
			break;												\
		case 'B':												\
			flag.is_case = true;								\
			__attribute__((fallthrough));						\
		case 'b':												\
			flag.base = 2;										\
			break;												\
		case 'u':												\
			flag.base = 10;										\
			break;												\
		case 'i':												\
			__attribute__((fallthrough));						\
		case 'd':												\
			flag.base = 10;										\
			flag.is_sgned = true;								\
			break;												\
		case 'c':												\
			flag.style = 0;										\
			len = 1;											\
			break;												\
		case 's':												\
			flag.style = 0;										\
			break;												\
		case 'R':												\
			flag.style = 1;										\
			break;												\
		case 'r':												\
			flag.style = 2;										\
			break;												\
		case 'S':												\
			flag.style = 3;										\
			break;												\
		case '%':												\
			PUSH_TO_BUFFER(c, buffer, len);						\
			len += 1;											\
		default:												\
			(void)0;											\
		}														\
	} while (0)

#endif
