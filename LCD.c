#include "LCD.h"

#include "Calculator.h"

#include "PLL.h"

#include <stdio.h>

#include <math.h>


void LCD_Init() {
  //Initialise Port A
  volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000003; // 1) enable A and B clocks
  delay = SYSCTL_RCGC2_R; // delay

  GPIO_PORTA_CR_R |= 0xFF; // allow changes to PA7-0       
  GPIO_PORTA_AMSEL_R &= 0x00; // 3) disable analog function
  GPIO_PORTA_PCTL_R &= 0x00000000; // 4) GPIO clear bit PCTL  
  GPIO_PORTA_DIR_R |= 0xFF; // 5) set all pins as outputs
  GPIO_PORTA_AFSEL_R &= 0x00; // 6) no alternate function
  GPIO_PORTA_PUR_R &= 0x00; // 7) disable pull up resitors on all pins
  GPIO_PORTA_DEN_R |= 0xFF; // 8) enable all pins as digital
  //Initialise Port B
  GPIO_PORTB_CR_R |= 0xFF; // allow changes to PB7-4       
  GPIO_PORTB_AMSEL_R &= 0x00; // 3) disable analog function
  GPIO_PORTB_PCTL_R &= 0x00000000; // 4) GPIO clear bit PCTL  
  GPIO_PORTB_DIR_R |= 0xFF; // 5) set all pins as outputs
  GPIO_PORTB_AFSEL_R &= 0x00; // 6) no alternate function
  GPIO_PORTB_PUR_R &= 0x00; // 7) disable pull up resitors on all pins
  GPIO_PORTB_DEN_R |= 0xFF; // 8) enable all pins as digital

  InitDisplayPort();
  SysTick_Wait_ms(100);
  lcdWelcome();
  lcdClearDisplay(); // display clear

}

void InitDisplayPort() { //sequence to initialise LCD
  SysTick_Wait_ms(20); //wait at least 15ms after power on
  lcd_RS = 0x00; //clear register select (1 = data register for read/write), 0 = instruction register for write)
  sendInstructionNibble(0b0011); //set bits DB7-4 = 0011
  SysTick_Wait_ms(5); //wait at least 4.1ms
  sendInstructionNibble(0b0011); //set bits DB7-4 = 0011
  SysTick_Wait_us(120); //wait at least 100us
  sendInstructionNibble(0b0011); //set bits DB7-4 = 0011
  SysTick_Wait_us(37); //wait standard delay 37us

  sendInstructionNibble(0b0010); //set 4 bit mode
  sendInstructionByte(0b00101000); // first function set again  to 4 bit mode then set set 0xNF00, N = 1 for two line display, F=0 for 5x8 font
  sendInstructionByte(0b00001000); // display off
  sendInstructionByte(0b00000001); // display clear
  sendInstructionByte(0b00000110); // entry mode set: set I/D=1, for increment, and S=0 so display does not shift
  sendInstructionByte(0b00001111); // Display on, Cursor on, Blinks On
  sendInstructionByte(0b00000110); //increase address by 1

}

void lcdPrintKey();
void lcdPrintKey() {}

void lcdWriteCommand(unsigned char c) {
  lcd_RS = 0x00; //set for writing instructions
  PB = (c << 4); //shift command up to PB4-7
  EnPulse_periods(36); //360ns delay
}

void lcdClearDisplay() { //clear and home the lcd display
  lcd_RS = 0x00; //set for writing instructions
  sendInstructionByte(0x01);
  sendInstructionByte(0x02); //return home
}

void lcdBackspace() { //delete last printed char

  sendInstructionByte(0b00010000); //shift cursor left
  SendDataByte(0b00100000); //write " "
  sendInstructionByte(0b00010000); //shift cursor left
  SysTick_Wait_ms(500); //longer delay to prevent bouncing

}

void SendDataNibble(unsigned long c) { //send 4 bits of datda
  lcd_RS = 0x01 << 3; //set for sending of Data
  PB = (c << 4); //set PB7-4 = c
  EnPulse_periods(23); //pulse Enable for 450ns = 23*20ns
  SysTick_Wait_ms(1);
}

void SendDataByte(unsigned long c) { //print char
  lcd_RS = 0x01 << 3; //set for sending of data
  SendDataNibble(c >> 4); //send most significant nibble
  SendDataNibble(c); //send least significant nibble
  SysTick_Wait_ms(1);
}

void sendInstructionNibble(unsigned long c) { //send 4 bits of instrucitons
  lcd_RS = 0x00; //set for sending of instructions
  PB = (c << 4); //set PB7-4 = c
  EnPulse_periods(23); //pulse Enable for 450ns = 23*20ns
  SysTick_Wait_ms(1);
}

void sendInstructionByte(unsigned long c) { //send 8 bits of instrucitions
  lcd_RS = 0x00; //set for sending of instructions !!!!CHECK**!!!
  sendInstructionNibble(c >> 4); //send most significant nibble
  sendInstructionNibble(c); //send least significant nibble
  SysTick_Wait_ms(1);
}

void EnPulse_us(unsigned long pw_us) {
  lcd_En = 0x01 << 2; //NEED TO SHIFT!?????
  SysTick_Wait_us(pw_us);
  lcd_En = 0x00;
  SysTick_Wait_ms(1);
}

void EnPulse_ms(unsigned long pw_ms) {
  lcd_En = 0x01 << 2; //NEED TO SHIFT!
  SysTick_Wait_ms(pw_ms);
  lcd_En = 0x00;
  SysTick_Wait_ms(1);
}

void EnPulse_periods(unsigned long periods) {
  lcd_En = 0x01 << 2; //NEED TO SHIFT!
  SysTick_Wait_Periods(periods);
  lcd_En = 0x00;
  SysTick_Wait_ms(1);
}

void lcdWelcome() {
  printString("WELCOME\0");
  SysTick_Wait_ms(200);
}

void
lcdPrintFloat(float number, int dp) { //accepts number and desired number of decimal places to print

  if (number == 0) {
    printCalcChar('0');
  } else {
    int maxLength = getFloatLength(number, dp); //determine buffer size needed by snprintf
    char charArray[maxLength]; // initialise char array
    floatToCharArray(number, charArray, maxLength, dp); //converts float to character array

    int i;
    for (i = 0; i < maxLength; ++i) //print out characters in array
    {
      printCalcChar(charArray[i]);
    }
  }
}

float
roundToDP(float number, int dp) { //requires math.h
  float x = pow(10, dp); //x= 10 to the power of the number of dp
  return round(number * x) / x; //multiply by x to shift the decimal point to the right by dp places, then round to a whole number, then shift back again
}

char *
  floatToCharArray(float number, char * charArray, int bufferSize, int dp) { //converts a float to a printable character array
    //sprintf reformats the float as a character array, and returns the length of the array 
    int formattedLength = sprintf(charArray, "%.*f", dp + 1, number);

    //remove trailing zeros:
    int i = formattedLength - 1; //if final char is a zero, remove
    while (charArray[i] == '0') {
      charArray[i] = '\0';
      i--; //decrement i
    }
    //remove trailing decimal point
    if (charArray[i] == '.') //if final char is a dp, remove it
    {
      charArray[i] = '\0';
    }

    return charArray; //return char array
  }

int
getFloatLength(float number, int dp) { //determines the number of chars in needed to represent the float, including - sign and dp
  int length = 0;
  if (number < 0) //if number is negative
  {
    length++; //add 1 for the minus sign
    number = -number; //convert to postive for rest of calculations
  }
  if (number != (int) number) //if number has a decimal component
  {
    length += dp + 1; //add number of decimal places, and one for the decimal point

  };
  while (number >= 1) // determine order magnitude by dividing by 10 until < 
  {
    number /= 10;
    length++;
  }
  return length; //return length
}

void printString(char* str){
int j=0;
	while(str[j]!='\0'){
	SendDataByte(str[j]);
		j++;
	}
}