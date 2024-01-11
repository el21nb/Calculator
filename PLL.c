#include "PLL.h"

void SysTick_Init(void) { //initialise SysTick
  NVIC_ST_CTRL_R = 0; //disable SysTick during setup
  NVIC_ST_RELOAD_R = 0xFFFFFF; // max reload value
  NVIC_ST_CURRENT_R = 0; // write to current to clera it            
  NVIC_ST_CTRL_R = 0x5; // enable SysTick with core clock
}

void PLLInit(void) { //initialse PLL
  SYSCTL_RCC2_R |= 0x80000000; // USERCC2
  SYSCTL_RCC2_R |= 0x00000800; // bypass PLL while initializing
  SYSCTL_RCC_R = (SYSCTL_RCC_R & ~0x000007C0) // clear XTAL field
    +
    0x00000540; // 10101 --> 16 MHz crystal
  SYSCTL_RCC2_R &= ~0x00000070; // configure main oscillator source
  SYSCTL_RCC2_R &= ~0x00002000; // activate PLL by clearing PWRDN
  SYSCTL_RCC2_R |= 0x40000000; // set system divider for 400 MHz PLL
  SYSCTL_RCC2_R = (SYSCTL_RCC2_R & ~0x1FC00000) // clear system clock divider
    +
    (5 << 22); // n = 5 for 50 MHz clock
  while ((SYSCTL_RIS_R & 0x00000040) == 0) {}; // wait for PLLRIS bit
  SYSCTL_RCC2_R &= ~0x00000800; //clear BYPASS to enable
}

void SysTick_Wait_Periods(unsigned long delay_periods) { //input unit is 50MHz periods = 20ns
  NVIC_ST_RELOAD_R = delay_periods - 1; // number of counts
  NVIC_ST_CURRENT_R = 0; // clear CURRENT reg
  while ((NVIC_ST_CTRL_R & 0x00010000) == 0) { // wait for count flag
  }
}

void SysTick_Wait_s(unsigned long delay_s) {
  NVIC_ST_RELOAD_R = (delay_s * 50000000) - 1; //number of counts = (number of s * periods per s )-1
  NVIC_ST_CURRENT_R = 0; // clear CURRENT reg
  while ((NVIC_ST_CTRL_R & 0x00010000) == 0) { // wait for count flag
  }
}

void SysTick_Wait_ms(unsigned long delay_ms) {
  NVIC_ST_RELOAD_R = (delay_ms * 50000) - 1; //number of counts = (number of ms * periods per ms )-1
  NVIC_ST_CURRENT_R = 0; // clear CURRENT reg
  while ((NVIC_ST_CTRL_R & 0x00010000) == 0) { // wait for count flag
  }
}

void SysTick_Wait_us(unsigned long delay_us) {
  NVIC_ST_RELOAD_R = (delay_us * 50) - 1; //number of counts = (number of us * periods per s )-1
  NVIC_ST_CURRENT_R = 0; // clear CURRENT reg
  while ((NVIC_ST_CTRL_R & 0x00010000) == 0) { // wait for count flag
  }
}