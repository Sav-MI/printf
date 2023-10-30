#include "main.h"
#include <stdio.h>

int main(void){
	int o, p;

	p = printf("%s", "\0");
	putchar('\n');
	o = _printf("%s", "\0");
	putchar('\n');

	printf("%d, %d\n", p, o);

	return (0);
}
