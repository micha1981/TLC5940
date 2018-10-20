
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



void SendGsDataToTLC(void);
void SendDcDataToTLC(void);
void setLine(char colour, int section, int row, int value);
void TLC5940_Init(void);
void setLightingPeriod(int counter);
void setLineRGB(int section, int row, int R, int G, int B);
void loadDcTable(void);
void setDCLine(char colour, int section, int row, int value);
void setAllRGB(int R, int G, int B);
int * randomChange(int *RgbRandomTable, int maxLight);
int * getLineRGBvalues(int section, int row);
void shiftUp(void);
