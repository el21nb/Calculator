#ifndef CALCULATOR_H
#define CALCULATOR_H

unsigned long Calc_Area(unsigned long l, unsigned long w);

typedef enum {
  OP,
  NUM
}
ElementType;

typedef struct {
  ElementType type;
  float value;
  char op;
}
OutputElement;

typedef struct {
  int pfPsn;
  float num;
}
numStruct;

typedef struct {
  int pfPsn;
  char op;
}
opStruct;

typedef struct {
  int filled;
  float value;
}
postFixStackElement;

typedef struct {
  float value;
  int length;
}
NumberInfo;


//functions in calling order:
void calculatorFunction(); //top level function, calls getCalculation and evaluateCalculation in an infinite loop

char * getCalculation(); //retrieves calculation string from keypad inputs. Uses appendBuffer and isKey
int isValidInput(char * calculation); //check if calculation string is valid
void invalidInput(); //print error message if input is invalid
void appendBuffer(char * buffer, char c); //appends a character to a buffer while building calculation string
int isKey(char k); //checks that the keypad input is valid for the calculation

float evaluateCalculation(char * calculation, float ans); //evaluates calculation string, calls shuntingYard and evaluatePostfix

OutputElement * shuntingYard(char * calculation, opStruct * opOutArray, numStruct * numOutArray, float ans); //converts prefix calculation string to a postfix expression. Returns an array of OutputElement structs. Calls:

int isNumber(char c); //check if char is a number or '.'
int isOperator(char o); //check if char is + - / * or ^
int getPrecedence(char op); //check precedence of operator
char getAssociativity(char op); //check if operator is left or right associative
NumberInfo extractFloat(const char * inputQueue, int i); //identifies a number, converts it to a float, returns the float and number length in a struct
float arrayToFloat(const char * inputString);

float evaluatePostFix(OutputElement * postFixArray); //calculates the final result from the postFix expression. Calls calculate() for each pair of operands.
float calculate(float operand1, float operand2, char operation); //calculate result of two operands and an operator

#endif