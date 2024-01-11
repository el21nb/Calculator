#ifndef KEYPAD_H
#define KEYPAD_H

#include "LCD.h"
#include "PLL.h"
#include <string.h>

//clock define:
#define SYSCTL_RCGC2_R 				(*((volatile unsigned long*)0x400FE108)) 

//PORT D register defines;
#define GPIO_PORTD_DATA_R 		(*((volatile unsigned long*)0x400073FC))
#define GPIO_PORTD_DIR_R 			(*((volatile unsigned long*)0x40007400))
#define GPIO_PORTD_AFSEL_R		(*((volatile unsigned long*)0x40007420))
#define GPIO_PORTD_PUR_R 			(*((volatile unsigned long*)0x40007510))
#define GPIO_PORTD_DEN_R 			(*((volatile unsigned long*)0x4000751C)) 
#define GPIO_PORTD_CR_R 			(*((volatile unsigned long*)0x40007524))
#define GPIO_PORTD_AMSEL_R 		(*((volatile unsigned long*)0x40007528))
#define GPIO_PORTD_PCTL_R 		(*((volatile unsigned long*)0x4000752C))
#define GPIO_PORTD_LOCK_R 		(*((volatile unsigned long*)0x40007520))

//PORT E register defines:
#define GPIO_PORTE_DATA_R 		(*((volatile unsigned long*)0x400243FC))
#define GPIO_PORTE_DIR_R 			(*((volatile unsigned long*)0x40024400))
#define GPIO_PORTE_AFSEL_R		(*((volatile unsigned long*)0x40024420))
#define GPIO_PORTE_PUR_R 			(*((volatile unsigned long*)0x40024510))
#define GPIO_PORTE_PDR_R 			(*((volatile unsigned long*)0x40024514))
#define GPIO_PORTE_DEN_R 			(*((volatile unsigned long*)0x4002451C))
#define GPIO_PORTE_CR_R 			(*((volatile unsigned long*)0x40024524))
#define GPIO_PORTE_AMSEL_R 		(*((volatile unsigned long*)0x40024528))
#define GPIO_PORTE_PCTL_R 		(*((volatile unsigned long*)0x4002452C))

//define columns:
#define Cols  (*((volatile unsigned long*)0x40007039)) //all columns
#define Col1	(*((volatile unsigned long*)0x40007004)) //PD0
#define Col2	(*((volatile unsigned long*)0x40007008)) //PD1
#define Col3	(*((volatile unsigned long*)0x40007010)) //PD2
#define Col4	(*((volatile unsigned long*)0x40007020)) //PD3
	
//define rows:
#define Rows  (*((volatile unsigned long*)0x40004039)) //all rows
#define Row1	(*((volatile unsigned long*)0x40024004)) //PE0
#define Row2	(*((volatile unsigned long*)0x40024008)) //PE1
#define Row3	(*((volatile unsigned long*)0x40024010)) //PE2
#define Row4	(*((volatile unsigned long*)0x40024020)) //PE3

void Keypad_Init(); //set up portD and port E and matrix
char readKeypad(); // reutrns the key pressed in the keypad matrix
char decodeKeyPress(char k); //returns the valeu of the keypad press
char readCalcKeypad(void); //read keypad and return calculator function char
char getKey(void); //fixes key misreads by waiting for two consecutive same reads
void printKey(char key); //prints out the char originally printed on the keypad
void printInt(int num); //prints an integer
void printCalcChar(char key); //prints a character on the modified calculator keypad, including shifted chars
void testKeypad(void); //test the keypad by reading a press and printing the char written on the key
#endif
