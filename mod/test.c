#include "main.h"
#include <stdio.h>

int main(void){
	int o, p;

	p = printf("%c%\0,true", '5');
	putchar('\n');
	o = _printf("%c%\0,true", '5', '5');
	putchar('\n');

	printf("%d, %d\n", p, o);
}
