#include "main.h"
#include <stdio.h>

int main(void){
	int o, p;

	p = printf("%");
	putchar('\n');
	o = _printf("%");
	putchar('\n');

	printf("%d, %d\n", p, o);

	return (0);
}
