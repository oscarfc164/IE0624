#include <pic14/pic12f683.h>

//To compile:
//sdcc -mpic14 -p16f675 blink.c
 
//To program the chip using picp:
//Assuming /dev/ttyUSB0 is the serial port.
 
//Erase the chip:
//picp /dev/ttyUSB0 16f887 -ef
 
//Write the program:
//picp /dev/ttyUSB0 16f887 -wp blink.hex
 
//Write the configuration words (optional):
//picp /dev/ttyUSB0 16f887 -wc 0x2ff4 0x3fff
 
//Doing it all at once: erasing, programming, and reading back config words:
//picp /dev/ttyUSB0 16f887 -ef -wp blink.hex -rc
 
//To program the chip using pk2cmd:
//pk2cmd -M -PPIC16f887 -Fblink.hex

typedef unsigned int word;
word __at 0x2007 __CONFIG = (_WDT_OFF & _WDTE_OFF);
 
void delay (unsigned inttiempo);

void lanzar_dado(unsigned int numero);

int gen_random(); 

unsigned int xo = 12345;

void main(void)

{
    TRISIO = 0b00100000; //Poner todos los pines como salidas
	GPIO = 0x00;
 
    unsigned int time = 100;
 
    //Loop forever
    while ( 1 )
    {
		if(GP5 == 0){
			unsigned int rand = gen_random();
			if(rand > 6){
				rand = (rand % 6) + 1;
				lanzar_dado(rand);
			} else {
				lanzar_dado(rand);
			}
			delay(100);
		}}}

void delay(unsigned int tiempo)
{
	unsigned int i;
	unsigned int j;

	for(i=0;i<tiempo;i++)
	  for(j=0;j<1275;j++);
}

int gen_random(){
	int p = 11;
	int q = 19;
	int M = p*q;

	xo = (xo*xo) % M;

	return xo;
}

void lanzar_dado(unsigned int numero){
	switch (numero)
	{
	case 1:
		GPIO = 0x01;
		break;
	case 2:
		GPIO = 0x02;
		break;
	case 3:
		GPIO = 0x03;
		break;
	case 4: 
		GPIO = 0x04;
		break;
	case 5: 
		GPIO = 0x05;
		break;
	case 6:
		GPIO = 0x06;
		break;
	
	default:
		GPIO = 0x00;
		break;
	}
}