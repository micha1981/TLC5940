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

uint8_t GSmatrix[24];




void SpiGS(void){
	PIN_down(XLAT_PORT, XLAT_PIN); //latch
		PIN_down(VPRG_PORT, VPRG_PIN); //select GS registry
		PIN_down(BLANK_PORT, BLANK_PIN);

	SPCR |= (1<<SPE)|(1<<MSTR);

	for (int i = 0; i < 24; i++) {
		SPDR = GSmatrix[i];
		while(!(SPSR &(1<<SPIF)));
	}
	PIN_Latch(XLAT_PORT, XLAT_PIN);
		_delay_ms(10);


}


void setLine(int rgbcolour, int row, int value){

	uint8_t high;
	uint8_t low;

	if(row==0){
	GSmatrix[row]=(value>>4);
	low=GSmatrix[row+1]&0x0F;
	high=value<<4;
	GSmatrix[row+1]=low|high;

	}


//	     0  |   1    |   2         |     3       |   4  |
//	111111111111  1111111111111 111111111111 111111111111
//	   0               1            2             3
//		4				5				6			7
//	   01              12           23            34

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

	setLine(0,0,3000);
	SpiGS();
	while (1) {
		int GSCLK = 0;
		for (GSCLK = 0; GSCLK < 4096; ++GSCLK) {
			PIN_Latch(GSCLK_PORT, GSCLK_PIN);
		}
		PIN_Latch(BLANK_PORT, BLANK_PIN);

	}

	return 0;
}

