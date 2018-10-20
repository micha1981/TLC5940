#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "tlc5940.h"

int main(void) {
	TLC5940_Init();
	loadDcTable();

	SendDcDataToTLC();
	int maxLight=10;
	int RGBTable[6]={0,0,0,0,0,0};
	int *pointer;
	pointer=RGBTable;

	setAllRGB(3000,0,0);
	while (1) {





		pointer=randomChange(pointer,maxLight);

		setLineRGB(0,0,RGBTable[0],RGBTable[1],RGBTable[2]);

		SendGsDataToTLC();
			shiftUp();



		setLightingPeriod(5);



	}
	return 0;
}

