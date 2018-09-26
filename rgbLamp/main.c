#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#define DCPRG_PIN PD2
#define GSCLK_PIN PD3
#define VPRG_PIN PD4
#define XLAT_PIN PB1
#define BLANK_PIN PB2
#define SIN_PIN PB3
#define SCLK_PIN PB5

#define DCPRG_PORT PORTD
#define GSCLK_PORT PORTD
#define VPRG_PORT PORTD
#define XLAT_PORT PORTB
#define BLANK_PORT PORTB
#define SIN_PORT PORTB
#define SCLK_PORT PORTB

#define PIN_up(port, pin) ((port) |=(1<<(pin)))

#define PIN_down(port, pin) ((port) &= ~(1 << (pin)));

#define PIN_Latch(port, pin) do { PIN_up((port), (pin)); \
									PIN_down((port), (pin)); \
                         } while (0)

uint8_t GSmatrix[6][4];




void SendGsDataToTLC(void){
	PIN_down(XLAT_PORT, XLAT_PIN); //latch
		PIN_down(VPRG_PORT, VPRG_PIN); //select GS registry
		PIN_down(BLANK_PORT, BLANK_PIN);

	SPCR |= (1<<SPE)|(1<<MSTR);

	for(int sect = 0; sect<4;sect++){
	for (int i = 0; i < 6; i++) {
		SPDR = GSmatrix[i][sect];
		while(!(SPSR &(1<<SPIF)));
	}
	}
	PIN_Latch(XLAT_PORT, XLAT_PIN);
		_delay_ms(10);


}
//
// row - one from 0-15 rows
// value - GS value 0-4095

void setLine(int section, int row, int value){

	uint8_t high;
	uint8_t low;

	switch (row) {
	case 0:
		GSmatrix[row][section] = (value >> 4);
		low = GSmatrix[row + 1][section] & 0x0F;
		high = value << 4;
		GSmatrix[row + 1][section] = low | high;
		break;
	case 1:
		high = GSmatrix[row][section] & 0xF0;
		low = value >> 8;
		GSmatrix[row][section] = low | high;
		GSmatrix[row + 1][section] = value;
		break;

	case 2:
		GSmatrix[row + 1][section] = value >> 4;
		low = GSmatrix[row + 2][section] & 0x0F;
		high = value << 4;
		GSmatrix[row + 2][section] = low | high;
		break;
	case 3:
		high = GSmatrix[row + 1][section] & 0xF0;
		low = value >> 8;
		GSmatrix[row + 1][section] = low | high;
		GSmatrix[row + 2][section] = value;
		break;
	}



//	     0  |   1    |   2    |     3 |   4    |  5      bytes
//	111111111111  111111111111 111111111111 111111111111 values

//		0               1            2            3   rows


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

int main(void) {
	TLC5940_Init();
	setLine(0, 0,4095);
	setLine(1, 1,4095);
	setLine(2, 2,4095);
	setLine(3, 3,4095);



	SendGsDataToTLC();
	while (1) {
		int GSCLK = 0;
		for (GSCLK = 0; GSCLK < 4096; ++GSCLK) {
			PIN_Latch(GSCLK_PORT, GSCLK_PIN);
		}

		PIN_Latch(BLANK_PORT, BLANK_PIN);

	}

	return 0;
}

