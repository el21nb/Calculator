#include "keypad.h"

#include "keypad.h"

#include <string.h>

char keyArray[4][4] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

char calcKeyArray[4][4] = {
  {'1','2','3','+'},
  {'4','5','6','-'},
  {'7','8','9','.'},
  {'=','0','C','^'}};

void Keypad_Init() {
  SYSCTL_RCGC2_R |= 0x08; // enable clock D
  GPIO_PORTD_LOCK_R = 0x4C4F434B; // unlock port D
  GPIO_PORTD_CR_R = 0xFF; // allow changes  
  GPIO_PORTD_AMSEL_R &= 0x00; // disable analog function
  GPIO_PORTD_PCTL_R = 0x00; // GPIO clear bit PCTL 
  GPIO_PORTD_DIR_R = 0xFF; // set all pins as outputs
  GPIO_PORTD_AFSEL_R = 0x00; // disable alternate functions   
  GPIO_PORTD_PUR_R &= 0x00; // disable pull up resitors on all pins
  GPIO_PORTD_DEN_R = 0xFF; // enable all pins as digital

  //Initialise Port E
  SYSCTL_RCGC2_R |= 0x10; // enable clock E
  GPIO_PORTE_CR_R = 0xFF; // allow changes
  GPIO_PORTE_AMSEL_R &= 0x00; // disable analog function
  GPIO_PORTE_PCTL_R = 0x00; // GPIO clear bit PCTL
  GPIO_PORTE_DIR_R = 0x00; // set all pins as inputs
  GPIO_PORTE_AFSEL_R = 0x00; // disable alternate functions
  GPIO_PORTD_PUR_R &= 0x00; // disable pull up resitors on all pins
  GPIO_PORTE_PDR_R = 0xFF; // enable pull down resitors on all pins
  GPIO_PORTE_DEN_R = 0x03F; // enable all pins as digital

}
void testKeypad(void) {
  while (1) {
    char k = readCalcKeypad();
    printKey(k);
    if (k == '7') {
      lcdBackspace();
    }
    SysTick_Wait_ms(20);
  }
}

char readCalcKeypad(void) { //returns symbol from calcKeyArray

  char keyPressed;
  int r;
  int c;
  int col;
  int row;
  GPIO_PORTD_DATA_R = 0;
  SysTick_Wait_us(50);
  for (c = 0; c < 4; c++) { //set columns high one by one
    GPIO_PORTD_DATA_R = 0x01 << c;
    SysTick_Wait_us(50);
    for (r = 0; r < 4; r++) { //check rows 1 by 1
      if (GPIO_PORTE_DATA_R & (0x01 << r)) { //if row is high
        keyPressed = calcKeyArray[c][r]; //find char from the array
        GPIO_PORTD_DATA_R = 0;
        return keyPressed; //return keyPressed
      }
    }
  }
	return keyPressed;
}
char getKey(void){ //deals with keypad being read wrong by waiting until have two consecutive identical reads before returning
	char k = readCalcKeypad();
    SysTick_Wait_us(50);
   char   c= readCalcKeypad();
    SysTick_Wait_us(10);

	while (k != c) { //wait for CLEAR to be released before begin collecting entries
    k = readCalcKeypad();
    SysTick_Wait_us(50);
		c= readCalcKeypad();
    SysTick_Wait_us(10);
  }    
  return k;
}

void printCalcChar(char key) { //works with readCalcKeypad, to print calc functions
  switch (key) {
  case '1':
    SendDataByte(0x31);
    break;
  case '2':
    SendDataByte(0x32);
    break;
  case '3':
    SendDataByte(0x33);
    break;
  case '4':
    SendDataByte(0x34);
    break;
  case '5':
    SendDataByte(0x35);
    break;
  case '6':
    SendDataByte(0x36);
    break;
  case '7':
    SendDataByte(0x37);
    break;
  case '8':
    SendDataByte(0x38);
    break;
  case '9':
    SendDataByte(0x39);
    break;
  case '0':
    SendDataByte(0x30);
    break;
  case '*':
    SendDataByte(0x78);
    break;
  case '+':
    SendDataByte(0x2B);
    break;
  case '-':
    SendDataByte(0x2D);
    break;
  case '/':
    SendDataByte(0x2F);
    break;
  case '.':
    SendDataByte(0x2E);
    break;
  case '(':
    SendDataByte(0x28);
    break;
  case ')':
    SendDataByte(0x29);
    break;
  case '^':
    SendDataByte(0x5E);
    break;
  case '=':
    SendDataByte(0x3D);
    break;
  default:
    break;
  }
  SysTick_Wait_us(10);
}

void printKey(char key) { //works with readKeypad, to print original key labels

  switch (key) {
  case '1':
    SendDataByte(0x31);
    break;
  case '2':
    SendDataByte(0x32);
    break;
  case '3':
    SendDataByte(0x33);
    break;
  case 'A':
    SendDataByte(0x41);
    break;
  case '4':
    SendDataByte(0x34);
    break;
  case '5':
    SendDataByte(0x35);
    break;
  case '6':
    SendDataByte(0x36);
    break;
  case 'B':
    SendDataByte(0x42);
    break;
  case '7':
    SendDataByte(0x37);
    break;
  case '8':
    SendDataByte(0x38);
    break;
  case '9':
    SendDataByte(0x39);
    break;
  case 'C':
    SendDataByte(0x43);
    break;
  case '*':
    SendDataByte(0x2A);
    break;
  case '0':
    SendDataByte(0x30);
    break;
  case '#':
    SendDataByte(0x23);
    break;
  case 'D':
    SendDataByte(0x44);
    break;
  default:
    break;
  }
  SysTick_Wait_us(10);

}

void printInt(int num) { //works with readKeypad, to print original key labels

  switch (num) {
  case 1:
    SendDataByte(0x31);
    break;
  case 2:
    SendDataByte(0x32);
    break;
  case 3:
    SendDataByte(0x33);
    break;
  case 4:
    SendDataByte(0x34);
    break;
  case 5:
    SendDataByte(0x35);
    break;
  case 6:
    SendDataByte(0x36);
    break;
  case 7:
    SendDataByte(0x37);
    break;
  case 8:
    SendDataByte(0x38);
    break;
  case 9:
    SendDataByte(0x39);
    break;
  case 0:
    SendDataByte(0x30);
    break;
  default:
    break;
  }
  SysTick_Wait_us(10);

}