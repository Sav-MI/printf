#include <stdio.h>
#include <stdbool.h>

int main(void){
char hexcase, hexadec_aa, hexadec_ff;

 unsigned int temp_integ = 13;
 char temp_buff[125];

 int is_case = 1;
 int oo = 0;
  
 if (is_case == true)
   hexcase = is_case ? 55 : 87; 
 do {
   hexadec_ff = temp_integ & 0b1111;
   temp_buff[oo] = hexadec_ff > 9 ? hexcase + hexadec_ff : 48 + hexadec_ff;
   temp_integ >>= 4;
   oo++;
 }  while (temp_integ);

 while (oo){
   //   printf("%c", temp_buff[oo-1]);
   oo--;
 }
 // putchar('a' | (1u << 6));

 printf("% .9d,\n", 5);

 //space can have -+
 //if there is + and number is negative put it off
 //minus flag without width is useless put it off
 //space only works for positive numbers
 //- width prints to the left according to the number left after the main stuffs print
 //0 flag is ignored with -

 //sign comes before width
}
// 110001
// 001111
// 000001

/*0b110000
0b110001
0b110010
0b110011
0b110100
0b110101
0b110110
0b110111
0b111000
0b111001 */
