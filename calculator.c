#include <stdio.h>   // standard C library

#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <math.h>

#include "PLL.h"

#include "LCD.h"

#include "calculator.h"

void calculatorFunction() { //top level function. Infinite loop: reads input, checks validity, gets result, prints result, waits for key press
  float ans = 0;
	while (1) {
    char * calculation = getCalculation(); //get calculation from keypad inputs

    if (!isValidInput(calculation)) { //check calculation validity
      invalidInput();
    }
    ans = evaluateCalculation(calculation, ans);
    lcdPrintFloat(ans, 4); //specify number of decimal places
    SysTick_Wait_ms(1000);
    char k = readCalcKeypad();
    while (!isKey(k)) { //wait for any valid key return to clear LCD
      k = readCalcKeypad();
      SysTick_Wait_us(10);
    }
    lcdClearDisplay();
    while (k == 'C') { //wait for CLEAR to be released before begin collecting entries
      k = readCalcKeypad();
      SysTick_Wait_us(10);
    }
  }
}

char * getCalculation() { //returns a character array of the calulation
  //char *inputBuffer = (char *) malloc (100);	

  char inputBuffer[100] = {
    0
  }; // initialise inputBuffer to store entered characters
  int shift = 0; // initialise shift flag as unshifted
  int calc = 1; // initialise calculating flag as calculating
  
	while (calc && strlen(inputBuffer) < 99) {
    //char k = readCalcKeypad();
    char k = getKey();
    shift_LED = shift; //match shift LED to shift flag
    // cases for different keys:
    if (k == '^') { // shift
      shift = 1;
    } else if (k == '+' && shift) { //shifted + --> *
      printCalcChar('*');
      appendBuffer(inputBuffer, '*');
      shift = 0;
      SysTick_Wait_ms(500); //long delay to prevent double pressing

    } else if (k == '-' && shift) { //shifted - --> /
      printCalcChar('/');
      appendBuffer(inputBuffer, '/');
      shift = 0;
      SysTick_Wait_ms(500);
    } else if (k == '.' && shift) { //shifted . --> ^
      printCalcChar('^');
      appendBuffer(inputBuffer, '^');
      shift = 0;
      SysTick_Wait_ms(500);

    } else if (k == '3' && shift) { //shifted 3 --> (
      printCalcChar('(');
      appendBuffer(inputBuffer, '(');
      shift = 0;
      SysTick_Wait_ms(500);
    } else if (k == '6' && shift) { //shifted 6 --> )
      printCalcChar(')');
      appendBuffer(inputBuffer, ')');
      shift = 0;
      SysTick_Wait_ms(500);

    } else if (k == '9' && shift) { //shifted 0 --> ans
			printString("ANS\0");
			appendBuffer(inputBuffer,'A'); //append A to buffer
      shift = 0;
      SysTick_Wait_ms(500);
    } else if (k == 'C' && shift) { //shifted C --> CLEAR DISPLAY
      lcdClearDisplay();
      shift = 0;
      inputBuffer[0] = '\0'; //clear input buffer by setting first element to null character
    } else if (k == 'C') { //'C' --> backspace
      int length = strlen(inputBuffer);
      if (length > 0) { //if buffer not empty
        lcdBackspace(); //delete last char on lcd
        inputBuffer[length - 1] = '\0'; //delete last element from buffer by setting to null character
      } else {
        inputBuffer[0] = '\0'; //otherwise clear inputBuffer
      }
      SysTick_Wait_ms(500);
    } else if (k == '=') {
      int length = strlen(inputBuffer);
      if (length > 0) { //if buffer not empty
        printCalcChar('='); // = symbol not saved to buffer
        SysTick_Wait_ms(500);
        calc = 0;
      }
    }
    //standrad case: print char and store to buffer
    else if (isKey(k)) { //default case: another valid char has been read
      printCalcChar(k); //print char to lcd
      appendBuffer(inputBuffer, k); //add char to buffer
      SysTick_Wait_ms(500);
    }
  }
	char * result = (char * ) malloc(strlen(inputBuffer) + 1); //allocate memory for the resulting calculation string
  strcpy(result, inputBuffer); //copy inputBuffer to result
	int m=0;
	for(m=0; m<strlen(inputBuffer);m++){
	}
	
	return result;
}

void appendBuffer(char * buffer, char c) { //used by getCalculation function to store the input character to a buffer array
  int length = strlen(buffer);
  buffer[length] = c;
  buffer[length + 1] = '\0'; //terminate with null character
}

int isKey(char k) { //check that the char returned by getKey is a valid char
  if (k == '1' || k == '2' || k == '3' || k == '+' || k == '4' || k == '5' || k == '6' || k == '-' || k == '7' || k == '8' || k == '9' || k == '.' || k == '=' || k == '0' || k == 'C' || k == '^') {
    return 1;
  } else {
    return 0;
  }
};

int isValidInput(char * calculation) { //returns 1 if the calculation string is valid
  int i = 0;
  int valid = 1; //initialise valid flag as true
  int lefts = 0; //counter for left braces
  int rights = 0; //counter for right braces

  int len = strlen(calculation);
  if (calculation[0] == '/' || calculation[0] == '*' || calculation[0] == '.' || calculation[0] == '^' || calculation[0] == ')') { //if invalid first character
    valid = 0;
    return valid;
  }
  if (isOperator(calculation[len - 1]) || calculation[len - 1] == '.' || calculation[len - 1] == '(') { //if invalid final character
    valid = 0;
    return valid;
  }
  for (i = 0; i < len; i++) { //check number of left brackets = number of right brackets;

    if (calculation[i] == '(') {
      lefts++;
    }
    if (calculation[i] == ')') {
      rights++;
    }
  }
  if (lefts != rights) { //not matching number of left and right brackets
    valid = 0;
    return valid;
  }

  for (i = 0; i < len; i++) { //check if any operators are followed by other operators (except + or -)
    if (isOperator(calculation[i]) && (calculation[i + 1] == '*' || calculation[i + 1] == '/' || calculation[i + 1] == '^' || calculation[i + 1] == '.' || calculation[i + 1] == ')')) { //if an operator is followed by an invalid char
      valid = 0;
      return valid;

    }
  }
  return valid; //return validity flag
}

void invalidInput() { //prints INVALID INPUT to the lcd, then waits for a key press
  lcdClearDisplay();
  char * msg = "INVALID INPUT\0";
  int i;
  for (i = 0; i < strlen(msg); i++) { //send each char as a databyte
    SendDataByte(msg[i]);
  }
  char k = readCalcKeypad();
  while (!isKey(k)) { //wait for any valid key return to clear LCD
    k = readCalcKeypad();
    SysTick_Wait_us(10);
  }
  lcdClearDisplay();
  while (k == 'C') { //wait for CLEAR to be released before begin collecting entries
    k = readCalcKeypad();
    SysTick_Wait_us(10);
  }
  calculatorFunction(); //recall calculatorFunction to start over
}

float evaluateCalculation(char * calculation, float ans) { //accepts the calculation string, returns the final result, and prints to LCD
	numStruct * pfNums = (numStruct * ) malloc(50 * sizeof(numStruct)); //allocate memory for the output array which is an array of outputElement structs
  opStruct * pfOps = (opStruct * ) malloc(50 * sizeof(opStruct)); //allocate memory for the output array which is an array of outputElement structs
  OutputElement * postFixArray = shuntingYard(calculation, pfOps, pfNums, ans); //Shunting Yard to transform prefix expression to postfix
	float result = evaluatePostFix(postFixArray); //get result by evaluating the postfix expression
  return result;
}

OutputElement * shuntingYard(char * calculation, opStruct * opOutArray, numStruct * numOutArray, float ans) { //transforms input calculation string to a postfix expression
	//initialise array size counters:
  int outputQueueSize = 0;
  int opStackSize = 0;
  int opArrayi = 0;
  int numArrayi = 0;
  int i = 0; //initialise iterator
  int queueLen = strlen(calculation) + 1;

  //allocate memory for operator stack and output array
  char * opStack = (char * ) malloc(queueLen);
  OutputElement * outputArray = (OutputElement * ) malloc(100 * sizeof(OutputElement));

  while (calculation[i] != '\0') //until reach the null character in the input queue
  {
		if(calculation[i]=='A'){
		  outputArray[outputQueueSize].value = ans; //store the float value and type to the output array
      outputArray[outputQueueSize].type = NUM;
			outputQueueSize++;
			i++;
		}
    else if (isNumber(calculation[i])) { //if character is a number 
      NumberInfo number = extractFloat(calculation, i); //extract the whole float from the input queue
      outputArray[outputQueueSize].value = number.value; //store tne float value and type to the output array
      outputArray[outputQueueSize].type = NUM;
      numOutArray[numArrayi].num = number.value; //store the number to the postfix numbers array
      numOutArray[numArrayi].pfPsn = outputQueueSize;
      numArrayi++; //increment all counters
      outputQueueSize++;
      i += number.length; //increment along the calculation string, by the length of the number
    } else if (isOperator(calculation[i])) { //if character is an operator
			char o1 = calculation[i]; //set this operator to operator 1
      char o2 = opStack[opStackSize - 1]; //get operator 2 from the top of the operator stack
      if ((calculation[i] == '-' || calculation[i] == '+') && !isNumber(calculation[i - 1])) { //if this operator is + or -, and it does not come after a number, then it is a sign not an operator
        i++; //skip over the sign
      } else {
        while ((o2 != '\0') && //while o2 exists
          ((getPrecedence(o1) < getPrecedence(o2)) || //and o2 has greater precedence than o1
            (getAssociativity(o1) == 'L' //OR they have equal precedence but o1 is left associative
              &&
              getPrecedence(o1) == getPrecedence(o2)))) {
          outputArray[outputQueueSize].type = OP; //push o2 to the output array
          outputArray[outputQueueSize].op = o2;
          opOutArray[opArrayi].op = o2; //store o2 in the postfix operators array
          opOutArray[opArrayi].pfPsn = outputQueueSize;
          opArrayi++;
          outputQueueSize++;
          opStack[opStackSize - 1] = '\0'; //pop o2 off the operator stack
          opStackSize--; //decrement operaor stack size counter
          o2 = opStack[opStackSize - 1]; //get new o2 from new top of operator stack
        }
        opStack[opStackSize] = o1; //push o1 to the operator stack
        opStackSize++; //increase operator stack size counter
        i++;
      }
    } else if (calculation[i] == '(') { //else if left bracket
      opStack[opStackSize] = '('; //push to operator stack
      opStackSize++; //increase operator stack size counter
      i++;
    } else if (calculation[i] == ')') { //else if right bracket
      char o2 = opStack[opStackSize - 1]; //read o2 from the top of the operator stack

      while (o2 != '(') { //while the top fo the operator stack is not a left bracket, keep popping the top of the operator stack to the output queue
        outputArray[outputQueueSize].op = o2;
        outputArray[outputQueueSize].type = OP;
        outputQueueSize++;
        opStack[opStackSize - 1] = '\0';
        opStackSize--;
        o2 = opStack[opStackSize - 1]; //get new o2
      }
      if (o2 == '(') { //when find left bracket
        opStack[opStackSize - 1] = '\0'; //pop from operator stack and discard
        opStackSize--;
      } else {
        invalidInput(); //mismatched parenthesis
      }
      i++;
    }
  }
  while (opStackSize > 0) { //pop remaining elements off the opstack and push to output queue
    char o = opStack[opStackSize - 1];
    opStack[opStackSize - 1] = '\0';
    opStackSize--;
    outputArray[outputQueueSize].op = o;
    outputArray[outputQueueSize].type = OP;
    opOutArray[opArrayi].op = o;
    opOutArray[opArrayi].pfPsn = outputQueueSize;
    opArrayi++;
    outputQueueSize++;
  }
  free(calculation);
  return outputArray;
}

int isNumber(char c) { //check if char is a digit or decimal point
  return c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' ||
    c == '7' || c == '8' || c == '9' || c == '0' || c == '.';
}

int isOperator(char o) { //check if char is an operator
  return (o == '+' || o == '-' || o == '*' || o == '/' || o == '^');
}

int getPrecedence(char op) { //return operator precedence
  switch (op) {
  case '^':
    return 4;
  case '/':
  case '*':
    return 3;
  case '+':
  case '-':
    return 2;
  default:
    return 0;
  }
}

char getAssociativity(char op) { //return operator associativity L or R
  switch (op) {
  case '^':
    return 'R';
  case '-':
  case '+':
  case '/':
  case '*':
    return 'L';
  default:
    return 0;
  }
}

NumberInfo extractFloat(const char * inputQueue, int i) { //identifies a number within a string. Returns value and length in NumberInfo struct
  //initialisations
  NumberInfo result;
  char numberStr[50];
  int j = 0; //iterator for numberStr
  int isNegative = 0;
  int dots = 0;

  if (inputQueue[i - 1] == '-' && !isNumber(inputQueue[i - 2])) { //if the character before the number is + or -, and the character before that isn't a number, then the + or - is a sign
    isNegative = 1; //negative flag high
    numberStr[0] = '-'; //add the sign to the front of the number string
    j++; //increment along
  }
  while (isNumber(inputQueue[i])) {
    if (inputQueue[i] == '.') { //count number of decimal points
      dots++;
    }
    numberStr[j] = inputQueue[i]; //store char to number string
    j++;
    i++;
  }
  if (dots > 1) { //if more than one decimal point, invalid input
    invalidInput();
  }
  numberStr[j] = '\0';
  float number = arrayToFloat(numberStr); //convert string to float

  int length = strlen(numberStr);

  result.value = number; //set number value
  result.length = j - isNegative; //set length of number: if negative, subtract 1 for the - sign
  return result; //return NumberInfo struct
}

float arrayToFloat(const char * inputString) { //converts a string, e.g. "-0.251" to a float value
  float floatValue = 0; //initialise result = 0
  int sign = 1; //default sign =1, number postivive
  float multiplier = 0.1; //initialise multiplier to 0.1 for tenths
	int i = 0;
  //Check for sign at start of string
  if (inputString[i] == '-') { //if char is a negative sign , set sign to -1;
    sign = -1; //change sign to negative
    i++; //move to next inputString address
  } else if (inputString[i] == '+') {
    i++; //move to next inputString address
  }

  //process characters before the decimal point
  while ( inputString[i] != '\0' && ( inputString[i] >= '0' && inputString[i] <= '9')) { //iterate through until find decimal point or null character
    floatValue = floatValue * 10.0 + ( * inputString - '0'); //each time move along one digit, multiply stored value by 10 then add the current digit
    //inputString - '0' converts the ascii char value of a digit to its numeric value
    i++; //move to next character of input string
  }
  if ( inputString[i] == '.') {
    i++; //skip decimal point

    while ( inputString[i] != '\0' && ( inputString[i] >= '0' && inputString[i]<= '9')) {
      floatValue = floatValue + ( inputString[i] - '0') * multiplier; //each time move along one digit, multiply the digit by the decimal divider, then add to the stored result
      multiplier *= 0.1; //divide multiplier by 10
      i++;
    }
  }
  return sign * floatValue; //correct sign
}


float evaluatePostFix(OutputElement * postFixArray) { //calculates result from the postfix expression
  //intiialisations:
  float finalResult;
  float * stack = (float * ) malloc(50);
  int stackSize = 0;
  int ar = 0;
  while (postFixArray[ar].type == OP || postFixArray[ar].type == NUM) { //while the postfix array element has a defined type
    if (postFixArray[ar].type == NUM) { //if a number

      float number = postFixArray[ar].value;
      stack[stackSize] = number; //push number to stack
      stackSize++;
      ar++;
    } else if (postFixArray[ar].type == OP) { //if an operator
      char op = postFixArray[ar].op; //set as op
      float o2 = stack[stackSize - 1]; //pop number off top of stack and set to operand 2
      stack[stackSize - 1] = '\0';
      stackSize--;
      float o1 = stack[stackSize - 1]; //pop next number off top of stack and set to operand 1
      stack[stackSize - 1] = '\0';
      stackSize--;
      float result = calculate(o1, o2, op); //calculate result of operand 1, operator, operand 2
      stack[stackSize] = result; //push result to top of stack
      stackSize++;
      ar++;
    }
  }
  finalResult = stack[0]; //pop final result off stack
  free(stack);
  return finalResult;
}

float
calculate(float operand1, float operand2, char operation) //calculate result of two operands and an operator
{
  float result;
  switch (operation) //switch depending on operator
  {
  case '+':
    result = operand1 + operand2;
    break;
  case '-':
    result = operand1 - operand2;
    break;
  case '/':
    result = operand1 / operand2;
    break;
  case '*':
    result = operand1 * operand2;
    break;
  case '^':
    result = pow(operand1, operand2); //from math.h library
    break;
  default:
    result = 0.0; // default should not  be needed
  }
  return result;
}