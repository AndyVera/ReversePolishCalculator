
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#define MAXOP   100    /* max size of operand/operator */
#define NUMBER '0'     /* signal that a number was found */
#define MATH '1'
#define STACK '2'
#define VARIABLE '3'
#define MAXVAL  100
#define ENGLISH 26


size_t sp = 0;   // aka unsigned long -- printf using %zu
double val[MAXVAL];   // stack of values
double variables[ENGLISH];
char buf[BUFSIZ];
size_t bufp = 0;

//===========
int getch_(void) { return (bufp > 0) ? buf[--bufp] : getchar(); }
void ungetch_(int c) {
  if (bufp >= BUFSIZ) { fprintf(stderr, "ungetch: too many characters\n");  return; }
  buf[bufp++] = c;
}
//==========
int getop(char* s) {
  int i, c;
  while ((s[0] = c = getch_()) == ' ' || c == '\t') { }  // skip whitespace
  s[1] = '\0';
  
    if(c == '=' || c == '?'){
        i = 0;
        while(isalpha(s[++i] = c = getch_())){}
        s[i] = '\0';
        return VARIABLE;
    }
    
    else if(c == '@'){
        i = 0;
        while (isalpha(s[++i] = c = getch_())){}
        s[i] = '\0';
        return STACK;
    }
    
    if (isalpha(c)){
        i = 0;
        while(isalpha(s[++i] = c = getch_())){}
        s[i] = '\0';
        return MATH;
    }
  if (!isdigit(c) && c != '.') { return c; }  // if not a digit, return

  i = 0;
  if (isdigit(c)) {  // get digits before '.'
    while (isdigit(s[++i] = c = getch_())) { }
  }
  if (c == '.') {    // get digits after decimal (if any)
    while (isdigit(s[++i] = c = getch_())) { }
  }
  s[i] = '\0';
  if (c != EOF) { ungetch_(c); }
  return NUMBER;      // return type is NUMBER, number stored in s
}
//=========
double pop(void) {
  if (sp == 0) { fprintf(stderr, "stack underflow\n");  return 0.0; }
  return val[--sp];
}
//==========
void push(double f) {
  if (sp == MAXVAL) { fprintf(stderr, "stack overflow -- can't push %g\n", f);  return; }
  val[sp++] = f;
}
//=========
void math(char* s){
    double op1, op2, result = 0;

    if(strcmp(s, "pow") == 0){
        op2 = pop();
        op1 = pop();
        result = pow(op1, op2);
    }
    else if(strcmp(s, "exp") == 0){
        result = exp(pop());
    }
    else if (strcmp(s, "sine") == 0){
        result = sin(pop());
    }
    else if(strcmp(s, "cosine") == 0){
        result = cos(pop());
    }
    else if(strcmp(s, "sqrt") == 0){
        result = sqrt(pop());
    }
    printf("    %0.3f\n", result);
    push(result);
}
//=========
void variables_(char* s){
    if(*s == '='){
        variables[*(s+1) - 'A'] = pop();
    }
    else if(*s == '?'){
        push(variables[*(s+1) - 'A']);
    }
}
//=========
void stacks(char* s){
    ++s;
    double op1,op2, result = 0;

    if(strcmp(s, "clear") == 0){
        //to clear the stack so nothing is in it
        sp = 0;
        result = val[sp - 1];
    }
    
    else if(strcmp(s, "swap") == 0){
        op1 = pop();
        op2 = pop();
        
        push(op1);
        push(op2);

        result = val[sp - 1];
    }
    else if(strcmp(s, "top") == 0){
        //not popping the value just showing what the top value of stack is
        result = val[sp - 1];
    }
    else if(strcmp(s, "dup") == 0){
        //duplicates the top value by obtaining the top value without popping it and pushing it into the stack
        result = val[sp - 1];
        push(result);
    }
    printf("    %0.3f\n", result);
}
//========
void rpn(void) {
  int type;
  double op2;
  char s[BUFSIZ];
  
  while ((type = getop(s)) != EOF) {
    switch(type) {
      case '\n':     printf("\t%.8g\n", pop());  break;
      case NUMBER:   push(atof(s));              break;
      case MATH:     math(s);                    break;
      case VARIABLE: variables_(s);              break;
      case STACK:    stacks(s);                  break;
      case '+':      push(pop() + pop());        break;
      case '*':      push(pop() * pop());        break;
      case '-':      push(-(pop() - pop()));     break;
      case '/':
        if ((op2 = pop()) == 0.0) { fprintf(stderr, "divide by zero\n");  break; }
        push(pop() / op2);
        break;
      case '%':
            if((op2 = pop()) == 0.0){ fprintf(stderr, "divisor must not be zero\n"); break;}
            push(fmod(pop(), op2));
            break;
 
      default:
      fprintf(stderr, "unknown command %s\n", s);  break;
    }
  }
}
int main(int argc, const char * argv[]) {
  rpn();

  return 0;
}


