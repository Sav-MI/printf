#include "main.h"
#include <stdio.h>

int main(void){
	int o, p;

	p = printf("%s", NULL);
	putchar('\n');
	o = _printf("%s", NULL);
	putchar('\n');

	printf("%d, %d\n", p, o);
}
