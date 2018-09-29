#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "tlc5940.h"

int main(void) {
	TLC5940_Init();
	int sect=0,row=0;
	int vector = 1;
	int R=1;
	while (1) {

		setLineRGB(sect,row,200,200,0);
		SendGsDataToTLC();
		//setLineRGB(sect,row,0,0,0);
		R=R+30;

		if(R==4000)R=0;
		if(row<3){
		row=row+vector;
		}
		else{
			if (sect<3){sect++;}
			else{sect=0;}

		row=0;
		}
		setLightingPeriod(1);


		}
	return 0;
}

