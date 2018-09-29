#include "tlc5940.h"
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>


uint8_t GsBlueMatrix[6][4];
uint8_t GsRedMatrix[6][4];
uint8_t GsGreenMatrix[6][4];

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
