#ifndef LCD_H
#define LCD_H
/*LCD PIn Connections:
// Pin 1   2   3   4   5   6   7   8   9   10   11   12   13   14   15   16
//Name Vss Vdd V0  RS  R/W E   DB0 .....                       DB7  LED+ LED-
//GPIO GND VbusPot PA3 GND PA2 PB0 PB1 PB2 PB3  PB4  PB5  PB6  PB7  Vbus GND*/

//pre-processor directives:
#define SYSCTL_RCGC2_R  	(*((volatile unsigned long*)0x400FE108))

//pin outs:
#define lcd_RS (*((volatile unsigned long*)0x40004020)) //PA3
#define lcd_En (*((volatile unsigned long*)0x40004010)) //PA2
#define shift_LED (*((volatile unsigned long*)0x40005004)) //PB0
//#define DB0 	 (*((volatile unsigned long*)0x40005004)) //PB0
//#define DB1		 (*((volatile unsigned long*)0x40005008)) //PB1
//#define DB2		 (*((volatile unsigned long*)0x40005010)) //PB2
//#define DB3    (*((volatile unsigned long*)0x40005020)) //PB3
#define DB4 	 (*((volatile unsigned long*)0x40005040)) //PB4
#define DB5    (*((volatile unsigned long*)0x40005080)) //PB5
#define DB6    (*((volatile unsigned long*)0x40005100)) //PB6
#define DB7    (*((volatile unsigned long*)0x40005200)) //PB7
#define DB7654 (*((volatile unsigned long*)0x400053C0)) //PB7-4
#define PB     (*((volatile unsigned long*)0x400053FC)) //all port B pins

//Port A register defines
#define GPIO_PORTA_DATA_R 		(*((volatile unsigned long*)0x400043FC))
#define GPIO_PORTA_DIR_R 			(*((volatile unsigned long*)0x40004400))
#define GPIO_PORTA_AFSEL_R		(*((volatile unsigned long*)0x40004420))
#define GPIO_PORTA_PUR_R 			(*((volatile unsigned long*)0x40004510))
#define GPIO_PORTA_DEN_R 			(*((volatile unsigned long*)0x4000451C)) //NEED LOCK R????
#define GPIO_PORTA_CR_R 			(*((volatile unsigned long*)0x40004524))
#define GPIO_PORTA_AMSEL_R 		(*((volatile unsigned long*)0x40004528))
#define GPIO_PORTA_PCTL_R 		(*((volatile unsigned long*)0x4000452C))
//PORT B register defines
#define GPIO_PORTB_DATA_R 		(*((volatile unsigned long*)0x400053FC))
#define GPIO_PORTB_DIR_R 			(*((volatile unsigned long*)0x40005400))
#define GPIO_PORTB_AFSEL_R		(*((volatile unsigned long*)0x40005420))
#define GPIO_PORTB_PUR_R 			(*((volatile unsigned long*)0x40005510))
#define GPIO_PORTB_DEN_R 			(*((volatile unsigned long*)0x4000551C))
#define GPIO_PORTB_CR_R 			(*((volatile unsigned long*)0x40005524))
#define GPIO_PORTB_AMSEL_R 		(*((volatile unsigned long*)0x40005528))
#define GPIO_PORTB_PCTL_R 		(*((volatile unsigned long*)0x4000552C))
//port f defines
//#define GPIO_PORTC_DATA_R       (*((volatile unsigned long *)0x400063FC))
//#define GPIO_PORTC_DIR_R        (*((volatile unsigned long *)0x40006400))
//#define GPIO_PORTC_AFSEL_R      (*((volatile unsigned long *)0x40006420))
//#define GPIO_PORTC_PUR_R        (*((volatile unsigned long *)0x40006510))
//#define GPIO_PORTC_DEN_R        (*((volatile unsigned long *)0x4000651C))
//#define GPIO_PORTC_LOCK_R       (*((volatile unsigned long *)0x40006520))
//#define GPIO_PORTC_CR_R         (*((volatile unsigned long *)0x40006524))
//#define GPIO_PORTC_AMSEL_R      (*((volatile unsigned long *)0x40006528))
//#define GPIO_PORTC_PCTL_R       (*((volatile unsigned long *)0x4000652C))

//lcd defines:
void LCD_Init();
void lcdWelcome();
void lcdWriteCommand(unsigned char c);
void lcdClearDisplay(); //clear and home the lcd display
void lcdWriteData(unsigned char c);
void SendDataNibble(unsigned long c);
void SendDataByte(unsigned long c);
void sendInstructionNibble(unsigned long  c);
void sendInstructionByte(unsigned long  c);
void InitDisplayPort();
void EnPulse_us(unsigned long  pw_us);
void EnPulse_ms(unsigned long  pw_ms);
void EnPulse_periods(unsigned long periods);
void printString(char* str);

void lcdPrintFloat (float number, int dp);	//prints a float to the lcd by turning it into a char array
//lcdPrintFloat calls:
float roundToDP (float number, int dp);	//rounds the number to a specified number of decumal places
char *floatToCharArray (float number, char *charArray, int bufferSize, int dp);	//converts float to char array
int getFloatLength (float number, int dp);	//checks the size of char array needed


#endif