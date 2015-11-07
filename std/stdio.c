#ifndef MY_STDID_H
#define MY_STDID_H

#include <stdint.h>

// DO NOT MESS WITH AMD64, see ABI page 50, "3.5.7 Variable Argument List" for details
#include <stdarg.h>


// --------------------------------
extern void b_output(const char *str);
extern unsigned char b_input_key(void);
extern void reboot();

char getchar() {
	return b_input_key();
}

static void putchar(char c) {
	char scr[2];
	scr[0] = c;
	scr[1] = '\0';
	b_output(scr);
}

static void printchar(char **str, int c)
{
	if (str) {
		**str = c;
		++(*str);
	}
	else (void)putchar(c);
}

#define PAD_RIGHT 1
#define PAD_ZERO 2

static int prints(char **out, const char *string, int width, int pad)
{
	int pc = 0, padchar = ' ';

	if (width > 0) {
		int len = 0;
		const char *ptr;
		for (ptr = string; *ptr; ++ptr) ++len;
		if (len >= width) width = 0;
		else width -= len;
		if (pad & PAD_ZERO) padchar = '0';
	}
	if (!(pad & PAD_RIGHT)) {
		for ( ; width > 0; --width) {
			printchar (out, padchar);
			++pc;
		}
	}
	for ( ; *string ; ++string) {
		printchar (out, *string);
		++pc;
	}
	for ( ; width > 0; --width) {
		printchar (out, padchar);
		++pc;
	}

	return pc;
}

/* the following should be enough for x32x->64 bit int */
#define PRINT_BUF_LEN 60

static int printi(char **out, uint64_t i, int b, int sg, int width, int pad, int letbase)
{
	char print_buf[PRINT_BUF_LEN];
	char *s;
	int t, neg = 0, pc = 0;
	uint64_t u = i;

	if (i == 0) {
		print_buf[0] = '0';
		print_buf[1] = '\0';
		return prints (out, print_buf, width, pad);
	}

	//if (sg && b == 10 && i < 0) {
	//	neg = 1;
	//	u = -i;
	//}

	s = print_buf + PRINT_BUF_LEN-1;
	*s = '\0';

	while (u) {
		t = u % b;
		if( t >= 10 )
			t += letbase - '0' - 10;
		*--s = t + '0';
		u /= b;
	}

	if (neg) {
		if( width && (pad & PAD_ZERO) ) {
			printchar (out, '-');
			++pc;
			--width;
		}
		else {
			*--s = '-';
		}
	}

	return pc + prints (out, s, width, pad);
}

static int print(char **out, const char *format, va_list args )
{
	int width, pad, size=1;
	int pc = 0;
	char scr[2];

	for (; *format != 0; ++format) {
		if (*format == '%') {
			size=1;
			++format;
			width = pad = 0;
			if (*format == '\0') break;
			if (*format == '%') goto out;
			if (*format == '-') {
				++format;
				pad = PAD_RIGHT;
			}
			while (*format == '0') {
				++format;
				pad |= PAD_ZERO;
			}
			for ( ; *format >= '0' && *format <= '9'; ++format) {
				width *= 10;
				width += *format - '0';
			}
			if( *format == 'l' ) {
				size=2;
				format++;
			}
			if( *format == 's' ) {
				char *s = va_arg( args, char* );
				pc += prints (out, s?s:"(null)", width, pad);
				continue;
			}
			if( *format == 'd' ) {
				if (size==1)
					pc += printi (out, va_arg( args, uint32_t ), 10, 1, width, pad, 'a');
				else
					pc += printi (out, va_arg( args, uint64_t ), 10, 1, width, pad, 'a');
				continue;
			}
			if( *format == 'x' ) {
				if (size==1)
					pc += printi (out, va_arg( args, uint32_t ), 16, 0, width, pad, 'a');
				else
					pc += printi (out, va_arg( args, uint64_t ), 16, 0, width, pad, 'a');
				continue;
			}
			if( *format == 'X' ) {
				if (size==1)
					pc += printi (out, va_arg( args, uint32_t ), 16, 0, width, pad, 'A');
				else
					pc += printi (out, va_arg( args, uint64_t ), 16, 0, width, pad, 'A');
				continue;
			}
			if( *format == 'u' ) {
				if (size==1)
					pc += printi (out, va_arg( args, uint32_t ), 10, 0, width, pad, 'a');
				else
					pc += printi (out, va_arg( args, uint64_t ), 10, 0, width, pad, 'a');
				continue;
			}
			if( *format == 'c' ) {
				/* char are converted to int then pushed on the stack */
				scr[0] = va_arg( args, int );
				scr[1] = '\0';
				pc += prints (out, scr, width, pad);
				continue;
			}
		}
		else {
		out:
			printchar (out, *format);
			++pc;
		}
	}
	va_end( args );
	return pc;
}

int printf(const char *format, ...)
{
        va_list args;

        va_start( args, format );
        return print( 0, format, args );
}

int sprintf(char *out, const char *format, ...)
{
        va_list args;

        va_start( args, format );
        return print( &out, format, args );
}

void puts(char* msg) {
	b_output(msg);
}

int exit(int code) {
	printf("Exit with code=%d! Kernel no-where to exit!\n press ESC to reboot.\n", code);
	b_input_key();
	while(1) {};
	reboot();
	return code;
}


#endif
