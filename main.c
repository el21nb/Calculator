#include "PLL.h"
#include "LCD.h"
#include "keypad.h"
#include "calculator.h"

int main(void) {
  PLLInit();
  SysTick_Init();
  LCD_Init();
  Keypad_Init();
  calculatorFunction();
}

