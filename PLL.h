#ifndef PLL_H
#define PLL_H

//Defines for PLL
#define SYSCTL_RIS_R          (*((volatile unsigned long *)0x400FE050))	
#define SYSCTL_RCC_R          (*((volatile unsigned long *)0x400FE060))
#define SYSCTL_RCC2_R         (*((volatile unsigned long *)0x400FE070))	

//Defines for SysTick
#define NVIC_ST_CTRL_R        (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R      (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R     (*((volatile unsigned long *)0xE000E018))

void SysTick_Init(void);
void PLLinit(void);
void SysTick_Wait_Periods(unsigned long delay_periods);
void SysTick_Wait_s(unsigned long delay_s);
void SysTick_Wait_ms(unsigned long delay_ms);
void SysTick_Wait_us(unsigned long delay_us);

#endif