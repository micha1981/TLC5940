#include "tlc5940.h"
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>



uint8_t GsBlueMatrix[6][4];
uint8_t GsRedMatrix[6][4];
uint8_t GsGreenMatrix[6][4];

uint8_t DcBlueMatrix[3][4];
uint8_t DcRedMatrix[3][4];
uint8_t DcGreenMatrix[3][4];


void SendGsDataToTLC(void) {
	PIN_down(XLAT_PORT, XLAT_PIN); //latch
	PIN_down(VPRG_PORT, VPRG_PIN); //select GS registry
	PIN_down(BLANK_PORT, BLANK_PIN);

	SPCR |= (1 << SPE) | (1 << MSTR);

	for (int sect = 0; sect < 4; sect++) {
		for (int i = 0; i < 6; i++) {
			SPDR = GsGreenMatrix[i][sect];
			while (!(SPSR & (1 << SPIF)));
		}
	}
	for (int sect = 0; sect < 4; sect++) {
		for (int i = 0; i < 6; i++) {
			SPDR = GsRedMatrix[i][sect];
			while (!(SPSR & (1 << SPIF)));
		}
	}
	for (int sect = 0; sect < 4; sect++) {
		for (int i = 0; i < 6; i++) {
			SPDR = GsBlueMatrix[i][sect];
			while (!(SPSR & (1 << SPIF)));
		}
	}
	PIN_Latch(XLAT_PORT, XLAT_PIN);
}

void SendDcDataToTLC(void) {
	PIN_down(XLAT_PORT, XLAT_PIN);

	PIN_down(BLANK_PORT, BLANK_PIN);
	PIN_up(VPRG_PORT, VPRG_PIN); //select GS registry

	SPCR |= (1 << SPE) | (1 << MSTR) | (1<<SPR1)| (1<<SPR0);

	for (int sect = 0; sect < 4; sect++) {
		for (int i = 0; i < 3; i++) {
			SPDR = DcGreenMatrix[i][sect];
			while (!(SPSR & (1 << SPIF)));
		}
	}
	for (int sect = 0; sect < 4; sect++) {
		for (int i = 0; i < 3; i++) {
			SPDR = DcRedMatrix[i][sect];
			while (!(SPSR & (1 << SPIF)));
		}
	}
	for (int sect = 0; sect < 4; sect++) {
		for (int i = 0; i < 3; i++) {
			SPDR = DcBlueMatrix[i][sect];
			while (!(SPSR & (1 << SPIF)));
		}
	}
	PIN_Latch(XLAT_PORT, XLAT_PIN);
	_delay_ms(10);
	PIN_down(VPRG_PORT, VPRG_PIN);
}




// colour - r,g,b colour
// section - 0-3 section of lamp
// row - one from 0-3 rows in each section
// value - GS value 0-4095

void setLine(char colour, int section, int row, int value){

	uint8_t high;
	uint8_t low;

	switch (row) {
	case 0:
		switch (colour) {
		case 'r':
			GsRedMatrix[row][section] = (value >> 4);
			low = GsRedMatrix[row + 1][section] & 0x0F;
			high = value << 4;
			GsRedMatrix[row + 1][section] = low | high;
			break;
		case 'g':
			GsGreenMatrix[row][section] = (value >> 4);
			low = GsGreenMatrix[row + 1][section] & 0x0F;
			high = value << 4;
			GsGreenMatrix[row + 1][section] = low | high;
			break;
		case 'b':
			GsBlueMatrix[row][section] = (value >> 4);
			low = GsBlueMatrix[row + 1][section] & 0x0F;
			high = value << 4;
			GsBlueMatrix[row + 1][section] = low | high;
			break;
		}

		break;
	case 1:
		switch (colour) {
		case 'r':
			high = GsRedMatrix[row][section] & 0xF0;
			low = value >> 8;
			GsRedMatrix[row][section] = low | high;
			GsRedMatrix[row + 1][section] = value;
			break;
		}
		switch (colour) {
		case 'g':
			high = GsGreenMatrix[row][section] & 0xF0;
			low = value >> 8;
			GsGreenMatrix[row][section] = low | high;
			GsGreenMatrix[row + 1][section] = value;
			break;
		}
		switch (colour) {
		case 'b':
			high = GsBlueMatrix[row][section] & 0xF0;
			low = value >> 8;
			GsBlueMatrix[row][section] = low | high;
			GsBlueMatrix[row + 1][section] = value;
			break;
		}
		break;
	case 2:
		switch (colour) {
		case 'r':
			GsRedMatrix[row + 1][section] = value >> 4;
			low = GsRedMatrix[row + 2][section] & 0x0F;
			high = value << 4;
			GsRedMatrix[row + 2][section] = low | high;
			break;
		case 'g':
			GsGreenMatrix[row + 1][section] = value >> 4;
			low = GsGreenMatrix[row + 2][section] & 0x0F;
			high = value << 4;
			GsGreenMatrix[row + 2][section] = low | high;
			break;

		case 'b':
			GsBlueMatrix[row + 1][section] = value >> 4;
			low = GsBlueMatrix[row + 2][section] & 0x0F;
			high = value << 4;
			GsBlueMatrix[row + 2][section] = low | high;
			break;
		}
		break;
	case 3:
		switch (colour) {
		case 'r':
			high = GsRedMatrix[row + 1][section] & 0xF0;
			low = value >> 8;
			GsRedMatrix[row + 1][section] = low | high;
			GsRedMatrix[row + 2][section] = value;
			break;
		}
		switch (colour) {
		case 'g':
			high = GsGreenMatrix[row + 1][section] & 0xF0;
			low = value >> 8;
			GsGreenMatrix[row + 1][section] = low | high;
			GsGreenMatrix[row + 2][section] = value;
			break;
		}
		switch (colour) {
		case 'b':
			high = GsBlueMatrix[row + 1][section] & 0xF0;
			low = value >> 8;
			GsBlueMatrix[row + 1][section] = low | high;
			GsBlueMatrix[row + 2][section] = value;
			break;
		}
		break;
	}
}

void setDCLine(char colour, int section, int row, int value) {

	uint8_t high;
	uint8_t low;



	switch (row) {
	case 0:
		switch (colour) {
		case 'r':
			low = DcRedMatrix[row][section] & 0x03;
			high = value << 2;
			DcRedMatrix[row][section] = low | high;
			break;

			break;
		case 'g':
			low = DcGreenMatrix[row][section] & 0x03;
			high = value << 2;
			DcGreenMatrix[row][section] = low | high;
			break;
		case 'b':
			low = DcBlueMatrix[row][section] & 0x03;
			high = value << 2;
			DcBlueMatrix[row][section] = low | high;
			break;
		}
		break;

	case 1:
		switch (colour) {
		case 'r':
			high = DcRedMatrix[row - 1][section] & 0xFC;
			low = value >> 4;
			DcRedMatrix[row - 1][section] = low | high;
			high = 0;
			low = 0;
			low = DcRedMatrix[row][section] & 0x0F;
			high = value << 4;
			DcRedMatrix[row][section] = low | high;
			break;
		}
		switch (colour) {
		case 'g':
			high = DcGreenMatrix[row - 1][section] & 0xFC;
			low = value >> 4;
			DcGreenMatrix[row - 1][section] = low | high;
			high = 0;
			low = 0;
			low = DcGreenMatrix[row][section] & 0x0F;
			high = value << 4;
			DcGreenMatrix[row][section] = low | high;
			break;
		}
		switch (colour) {
		case 'b':
			high = DcBlueMatrix[row - 1][section] & 0xFC;
			low = value >> 4;
			DcBlueMatrix[row - 1][section] = low | high;
			high = 0;
			low = 0;
			low = DcBlueMatrix[row][section] & 0x0F;
			high = value << 4;
			DcBlueMatrix[row][section] = low | high;
			break;
		}
		break;
	case 2:

		switch (colour) {
		case 'r':
			high = DcRedMatrix[row - 1][section] & 0xF0;
			low = value >> 2;
			DcRedMatrix[row - 1][section] = low | high;
			high = 0;
			low = 0;
			low = DcRedMatrix[row][section] & 0x3F;
			high = value << 6;
			DcRedMatrix[row][section] = low | high;
			break;
		case 'g':
			high = DcGreenMatrix[row - 1][section] & 0xF0;
			low = value >> 2;
			DcGreenMatrix[row - 1][section] = low | high;
			high = 0;
			low = 0;
			low = DcGreenMatrix[row][section] & 0x3F;
			high = value << 6;
			DcGreenMatrix[row][section] = low | high;
			break;

		case 'b':
			high = DcBlueMatrix[row - 1][section] & 0xF0;
			low = value >> 2;
			DcBlueMatrix[row - 1][section] = low | high;
			high = 0;
			low = 0;
			low = DcBlueMatrix[row][section] & 0x3F;
			high = value << 6;
			DcBlueMatrix[row][section] = low | high;
			break;
		}
		break;

	case 3:
		switch (colour) {
		case 'r':
			high = DcRedMatrix[row - 1][section] & 0xC0;
			low = value;
			DcRedMatrix[row - 1][section] = low | high;
			break;
		}
		switch (colour) {
		case 'g':
			high = DcGreenMatrix[row - 1][section] & 0xC0;
			low = value;
			DcGreenMatrix[row - 1][section] = low | high;
			break;
		}
		switch (colour) {
		case 'b':
			high = DcBlueMatrix[row - 1][section] & 0xC0;
			low = value;
			DcBlueMatrix[row - 1][section] = low | high;
			break;
		}
		break;
	}
}




void TLC5940_Init(void) {
	DDRD |= (1 << DCPRG_PIN); //set as output
	DDRD |= (1 << GSCLK_PIN); //set as output
	DDRD |= (1 << VPRG_PIN); //set as output
	DDRB |= (1 << XLAT_PIN); //set as output
	DDRB |= (1 << BLANK_PIN); //set as output
	DDRB |= (1 << SIN_PIN); //set as output
	DDRB |= (1 << SCLK_PIN); //set as output
	PIN_down(GSCLK_PORT, GSCLK_PIN);
	PIN_down(SCLK_PORT, SCLK_PIN);
	PIN_up(DCPRG_PORT, DCPRG_PIN);
	PIN_up(VPRG_PORT, VPRG_PIN);
	PIN_down(XLAT_PORT, XLAT_PIN);
	PIN_up(BLANK_PORT, BLANK_PIN);
}


void setLightingPeriod(int counter) {
	for (int time = 0; time < counter; time++) {
		int GSCLK = 0;
		for (GSCLK = 0; GSCLK < 4096; ++GSCLK) {
			PIN_Latch(GSCLK_PORT, GSCLK_PIN);

		}
		PIN_Latch(BLANK_PORT, BLANK_PIN);
	}
}

void setLineRGB(int section, int row, int R, int G, int B){
setLine('r',section,row,R);
setLine('g',section,row,G);
setLine('b',section,row,B);
}

void setAllRGB(int R, int G, int B){
	int sect=0,row=0;
for(sect=0;sect<4;sect++){
	for(row=0;row<4;row++){
	setLineRGB(sect,row,R,G,B);
}
}
}

int * randomChange(int *RgbRandomTable, int maxLight){
	int changeSpeed=1;
if((*(RgbRandomTable)-*(RgbRandomTable+3)<=changeSpeed)&&(*(RgbRandomTable+1)-*(RgbRandomTable+4)<=changeSpeed)&&(*(RgbRandomTable+2)-*(RgbRandomTable+5)<=changeSpeed)){
	*(RgbRandomTable+3) = rand() % (maxLight*40);
	*(RgbRandomTable+4) = rand() % ((maxLight*40)-*(RgbRandomTable+3));
	*(RgbRandomTable+5) = rand() % (4000-*(RgbRandomTable+4)-*(RgbRandomTable+3));
}
if(*(RgbRandomTable)>*(RgbRandomTable+3)){
	*(RgbRandomTable)=*(RgbRandomTable)-changeSpeed;}
else{
	if(*(RgbRandomTable)<*(RgbRandomTable+3)){
		*(RgbRandomTable)=*(RgbRandomTable)+changeSpeed;}
}

if(*(RgbRandomTable+1)>*(RgbRandomTable+4)){
	*(RgbRandomTable+1)=*(RgbRandomTable+1)-changeSpeed;}
else{
	if(*(RgbRandomTable+1)<*(RgbRandomTable+4)){
		*(RgbRandomTable+1)=*(RgbRandomTable+1)+changeSpeed;}
}

if(*(RgbRandomTable+2)>*(RgbRandomTable+5)){
	*(RgbRandomTable+2)=*(RgbRandomTable+2)-changeSpeed;}
else{
	if(*(RgbRandomTable+2)<*(RgbRandomTable+5)){
		*(RgbRandomTable+2)=*(RgbRandomTable+2)+changeSpeed;}
}
return RgbRandomTable;
}


int  * getLineRGBvalues(int section, int row) {
	static int RGBvalues[3];
	int *pointer;
	pointer=RGBvalues;


		switch (row) {
		case 0:
			RGBvalues[0]=GsRedMatrix[0][section]<<4|GsRedMatrix[1][section]>>4;
			RGBvalues[1]=GsGreenMatrix[0][section]<<4|GsGreenMatrix[1][section]>>4;
			RGBvalues[2]=GsBlueMatrix[0][section]<<4|GsBlueMatrix[1][section]>>4;
			break;
		}
		switch (row) {
		case 1:
			RGBvalues[0]=((GsRedMatrix[1][section]&0x0F)<<8)|GsRedMatrix[2][section];
			RGBvalues[1]=((GsGreenMatrix[1][section]&0x0F)<<8)|GsGreenMatrix[2][section];
			RGBvalues[2]=((GsBlueMatrix[1][section]&0x0F)<<8)|GsBlueMatrix[2][section];
			break;
		}
		switch (row) {
		case 2:

			RGBvalues[0]=GsRedMatrix[3][section]<<4|GsRedMatrix[4][section]>>4;
			RGBvalues[1]=GsGreenMatrix[3][section]<<4|GsGreenMatrix[4][section]>>4;
			RGBvalues[2]=GsBlueMatrix[3][section]<<4|GsBlueMatrix[4][section]>>4;
			break;
		}
		switch (row) {
		case 3:
			RGBvalues[0]=((GsRedMatrix[4][section]&0x0F)<<8)|GsRedMatrix[5][section];
			RGBvalues[1]=((GsGreenMatrix[4][section]&0x0F)<<8)|GsGreenMatrix[5][section];
			RGBvalues[2]=((GsBlueMatrix[4][section]&0x0F)<<8)|GsBlueMatrix[5][section];
			break;
		}



	return pointer;
}


void shiftUp(void){
	int *pointer;
for (int sect = 3; sect >= 0; sect-- ) {

	for (int row = 3; row >= 0; row--) {
		pointer=getLineRGBvalues(sect,row);
		if(sect+row!=6){

			if(row==3){
				setLineRGB(sect+1,0,*(pointer),*(pointer+1),*(pointer+2));
			}else{
				setLineRGB(sect,row+1,*(pointer),*(pointer+1),*(pointer+2));
			}



		}
	}
}

}


void loadDcTable(void){
setDCLine('r',0,0,90);
setDCLine('r',0,1,90);
setDCLine('r',0,2,90);
setDCLine('r',0,3,90);
setDCLine('r',1,0,90);
setDCLine('r',1,1,90);
setDCLine('r',1,2,90);
setDCLine('r',1,3,90);
setDCLine('r',2,0,90);
setDCLine('r',2,1,90);
setDCLine('r',2,2,90);
setDCLine('r',2,3,90);
setDCLine('r',3,0,90);
setDCLine('r',3,1,90);
setDCLine('r',3,2,90);
setDCLine('r',3,3,90);

setDCLine('g',0,0,90);
setDCLine('g',0,1,90);
setDCLine('g',0,2,90);
setDCLine('g',0,3,90);
setDCLine('g',1,0,90);
setDCLine('g',1,1,90);
setDCLine('g',1,2,90);
setDCLine('g',1,3,90);
setDCLine('g',2,0,90);
setDCLine('g',2,1,90);
setDCLine('g',2,2,90);
setDCLine('g',2,3,90);
setDCLine('g',3,0,90);
setDCLine('g',3,1,90);
setDCLine('g',3,2,90);
setDCLine('g',3,3,90);

setDCLine('b',0,0,20);
setDCLine('b',0,1,20);
setDCLine('b',0,2,20);
setDCLine('b',0,3,20);
setDCLine('b',1,0,20);
setDCLine('b',1,1,20);
setDCLine('b',1,2,20);
setDCLine('b',1,3,20);
setDCLine('b',2,0,20);
setDCLine('b',2,1,20);
setDCLine('b',2,2,20);
setDCLine('b',2,3,20);
setDCLine('b',3,0,20);
setDCLine('b',3,1,20);
setDCLine('b',3,2,20);
setDCLine('b',3,3,20);
}
