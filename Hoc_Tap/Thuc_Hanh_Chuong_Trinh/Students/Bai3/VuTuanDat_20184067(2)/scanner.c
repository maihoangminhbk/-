/* Scanner
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "reader.h"
#include "charcode.h"
#include "token.h"
#include "error.h"
#include "scanner.h"


extern int lineNo;
extern int colNo;
extern int currentChar;

extern CharCode charCodes[];

/***************************************************************/

void skipBlank() {
  while ((currentChar != EOF) && (charCodes[currentChar] == CHAR_SPACE))
    readChar();
}

void skipComment() {
  /*In function getToken(): 
    case: '(' is divided into 3 case: 
    + case 1: * // comment --> HERE WE ARE (* --> need "*" of "*)"
    + case 2: . // index of array
  */

  // another readChar() was called right after this skipComment()
  while(currentChar != EOF && charCodes[currentChar] != CHAR_TIMES)
    readChar();

  readChar();
  if(currentChar != EOF && charCodes[currentChar] == CHAR_RPAR)
    readChar();
  else{
    error(ERR_ENDOFCOMMENT,lineNo,colNo);
  }

  /*
    if(nextChar != ')') error() -->  1-33 SB_RPAR the ")" remains
  */
}

Token* readIdentKeyword(void) {
  Token* finalToken = makeToken(TK_NONE,lineNo,colNo);
  finalToken->string[0] = (char)currentChar;
  int count = 1;
  readChar(); //--> move to next character, that's why count = 1;

  while(currentChar != EOF && ((charCodes[currentChar] == CHAR_LETTER) || (charCodes[currentChar] == CHAR_DIGIT))){
    finalToken->string[count++] = (char)currentChar;
    // start from 1 already.
    // condition: scanning ("digit" or "letter") || not EOF 
    readChar();
  }

  if(count > MAX_IDENT_LEN){
    error(ERR_IDENTTOOLONG,finalToken->lineNo,finalToken->colNo);
    /*
     before: error(ERR_IDENTTOOLONG,lineNo,colNo); 
     --> lineNo and colNo always change !!
     */
  }

  finalToken->string[count] = '\0';
  finalToken->tokenType = checkKeyword(finalToken->string);
  // check tokenType of finalToken is "Keyword" or not (VD: "var")
  // this function has 2 cases: "Ident" & "keyword"
				  
  if(finalToken->tokenType == TK_NONE){
    finalToken->tokenType = TK_IDENT;
    // if tokenType is not "Key word" --> it is an "identifier"
  }
  return finalToken;
}

Token* readNumber(void) {
 Token *finalToken = makeToken(TK_NUMBER,lineNo,colNo);
  int count = 0, hat = 0, i = 0;
  char floatString[MAX_IDENT_LEN+1];
  //readChar();
  //We're at the 1st digit. if readChar() --> lack a digit.

  while(currentChar != EOF && (charCodes[currentChar] == CHAR_DIGIT)){
    finalToken->string[count++] = (char)currentChar;
    readChar();
  }

  if(count > 10){
    printf("Exceeded value range of integer!");
    // -2,147,483,648 -->  2,147,483,647 (10 digits)
  }
  finalToken->string[count] = '\0';
  finalToken->value = atoi(finalToken->string);
  
  // currentChar != digit --> while stop --> currentChar == '.'   
  count = 0;
  // Does readChar() change value of "currentChar"????
  
  if(charCodes[currentChar] == CHAR_PERIOD){
    //if number = double/float
    readChar();
    while(currentChar != EOF && (charCodes[currentChar] == CHAR_DIGIT)){
      floatString[count++] = (char)currentChar;
      hat++;
      readChar();
    }

    floatString[count] = '\0';
    finalToken->f_value = atoi(floatString);
    for(i=0;i<hat;i++){
      finalToken->f_value /= 10;
    }
    finalToken->f_value += finalToken->value;

    strcat(finalToken->string,".");
    strcat(finalToken->string,floatString);
    finalToken->tokenType = TK_FLOAT;
  }
  return finalToken;
}

Token* readConstChar(void) {
  Token *token = makeToken(TK_CHAR, lineNo, colNo);

  readChar();
  if (currentChar == EOF) {
    token->tokenType = TK_NONE;
    error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
    return token;
  }
    
  token->string[0] = currentChar;
  token->string[1] = '\0';

  readChar();
  if (currentChar == EOF) {
    token->tokenType = TK_NONE;
    error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
    return token;
  }

  if (charCodes[currentChar] == CHAR_SINGLEQUOTE) {
    readChar();
    return token;
  } else {
    token->tokenType = TK_NONE;
    error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
    return token;
  }
}

Token* getToken(void) {
  Token *token;
  int ln, cn;

  if (currentChar == EOF) 
    return makeToken(TK_EOF, lineNo, colNo);

  switch (charCodes[currentChar]) {
  case CHAR_SPACE: skipBlank(); return getToken();
  case CHAR_LETTER: return readIdentKeyword();
  case CHAR_DIGIT: return readNumber();
  case CHAR_PLUS: 
    token = makeToken(SB_PLUS, lineNo, colNo);
    readChar(); // move to next char && before lineNo & colNo change

    if(currentChar != EOF && (charCodes[currentChar] == CHAR_EQ)){
      token->tokenType = SB_ASSIGN_PLUS;
      readChar();
    }
    return token;
    
  case CHAR_MINUS:
    // case 1: - || case 2: -=
    token = makeToken(SB_MINUS,lineNo,colNo);
    readChar(); // move to next char

    if(currentChar != EOF && (charCodes[currentChar] == CHAR_EQ)){
      token->tokenType = SB_ASSIGN_SUBTRACT;
      readChar();
    }
    return token;
    
  case CHAR_TIMES:
    // case 1: * || case 2: *=
    token = makeToken(SB_TIMES,lineNo,colNo);
    //before lineNo & colNo changed by function readChar();
    readChar(); // move to next char

    if(currentChar != EOF && (charCodes[currentChar] == CHAR_EQ)){
      token->tokenType = SB_ASSIGN_TIMES;
      readChar();
    }
    return token;
    
  case CHAR_SLASH:
    // case 1: / || case 2: /=
    token = makeToken(SB_SLASH,lineNo,colNo);
    readChar();

    if(currentChar != EOF && (charCodes[currentChar] == CHAR_EQ)){
      token->tokenType = SB_ASSIGN_DIVIDE;
      readChar();
    }
    return token;
    
  case CHAR_LT:
    // case 1: < || case 2: <=
    token = makeToken(SB_LT,lineNo,colNo);
    readChar();

    if(currentChar != EOF && (charCodes[currentChar] == CHAR_EQ)){
      token->tokenType = SB_LE;
      readChar();
    }
    return token;
    
  case CHAR_GT:
 // case 1: > || case 2: >=
    token = makeToken(SB_GT,lineNo,colNo);
    readChar();

    if(currentChar != EOF && (charCodes[currentChar] == CHAR_EQ)){
      token->tokenType = SB_GE;
      readChar();
    }
    return token;
    
  case CHAR_EQ: 
    token = makeToken(SB_EQ, lineNo, colNo);
    readChar(); 
    return token;
    
  case CHAR_EXCLAIMATION:
    // case 1: !=
    token = makeToken(TK_NONE,lineNo,colNo);
    ln = lineNo; cn = colNo;
    readChar();

    if(currentChar != EOF && (charCodes[currentChar] == CHAR_EQ)){
      token->tokenType = SB_NEQ;
      readChar();
      return token;
    }
    else{
      error(ERR_INVALIDSYMBOL,ln,cn);
    }
    
  case CHAR_COMMA:
    token = makeToken(SB_COMMA, lineNo, colNo);
    readChar(); 
    return token;
    
  case CHAR_PERIOD:
    // case1: '.' || case 2: '.)' --> index of array
    token = makeToken(SB_PERIOD,lineNo,colNo);
    readChar();
    if(currentChar != EOF && charCodes[currentChar] == CHAR_RPAR){
      token->tokenType = SB_RSEL;
      readChar(); 
      //RSEL ????
    }
    return token;
    
  case CHAR_SEMICOLON:
    token = makeToken(SB_SEMICOLON, lineNo, colNo);
    readChar(); 
    return token;
    
  case CHAR_COLON:
    token = makeToken(SB_COLON,lineNo,colNo);
    readChar();

    if(currentChar != EOF && (charCodes[currentChar] == CHAR_EQ)){
      token->tokenType = SB_ASSIGN;
      readChar();
    }
    return token;
    
  case CHAR_SINGLEQUOTE: return readConstChar();
    
  case CHAR_LPAR:
    ln = lineNo;
    cn = colNo;
    readChar();

    if (currentChar == EOF) 
      return makeToken(SB_LPAR, ln, cn);

    switch (charCodes[currentChar]) {
    case CHAR_PERIOD:
      readChar();
      return makeToken(SB_LSEL, ln, cn);
    case CHAR_TIMES:
      readChar();
      skipComment();
      return getToken();
    default:
      return makeToken(SB_LPAR, ln, cn);
    }
    
  case CHAR_RPAR:
    token = makeToken(SB_RPAR, lineNo, colNo);
    readChar(); 
    return token;
    
  default:
    token = makeToken(TK_NONE, lineNo, colNo);
    error(ERR_INVALIDSYMBOL, lineNo, colNo);
    readChar(); 
    return token;
  }
}

Token* getValidToken(void) {
  Token *token = getToken();
  while (token->tokenType == TK_NONE) {
    free(token);
    token = getToken();
  }
  return token;
}


/******************************************************************/

void printToken(Token *token) {

  printf("%d-%d:", token->lineNo, token->colNo);

  switch (token->tokenType) {
  case TK_NONE: printf("TK_NONE\n"); break;
  case TK_IDENT: printf("TK_IDENT(%s)\n", token->string); break;
  case TK_NUMBER: printf("TK_NUMBER(%s)\n", token->string); break;
  case TK_CHAR: printf("TK_CHAR(\'%s\')\n", token->string); break;
  case TK_FLOAT: printf("TK_FLOAT(\'%s\')\n",token->string);break;
  case TK_EOF: printf("TK_EOF\n"); break;

  case KW_PROGRAM: printf("KW_PROGRAM\n"); break;
  case KW_CONST: printf("KW_CONST\n"); break;
  case KW_TYPE: printf("KW_TYPE\n"); break;
  case KW_VAR: printf("KW_VAR\n"); break;
  case KW_INTEGER: printf("KW_INTEGER\n"); break;
  case KW_CHAR: printf("KW_CHAR\n"); break;
  case KW_ARRAY: printf("KW_ARRAY\n"); break;
  case KW_OF: printf("KW_OF\n"); break;
  case KW_FUNCTION: printf("KW_FUNCTION\n"); break;
  case KW_PROCEDURE: printf("KW_PROCEDURE\n"); break;
  case KW_BEGIN: printf("KW_BEGIN\n"); break;
  case KW_END: printf("KW_END\n"); break;
  case KW_CALL: printf("KW_CALL\n"); break;
  case KW_IF: printf("KW_IF\n"); break;
  case KW_THEN: printf("KW_THEN\n"); break;
  case KW_ELSE: printf("KW_ELSE\n"); break;
  case KW_WHILE: printf("KW_WHILE\n"); break;
  case KW_DO: printf("KW_DO\n"); break;
  case KW_FOR: printf("KW_FOR\n"); break;
  case KW_TO: printf("KW_TO\n"); break;

  case SB_SEMICOLON: printf("SB_SEMICOLON\n"); break;
  case SB_COLON: printf("SB_COLON\n"); break;
  case SB_PERIOD: printf("SB_PERIOD\n"); break;
  case SB_COMMA: printf("SB_COMMA\n"); break;
  case SB_ASSIGN: printf("SB_ASSIGN\n"); break;
  case SB_EQ: printf("SB_EQ\n"); break;
  case SB_NEQ: printf("SB_NEQ\n"); break;
  case SB_LT: printf("SB_LT\n"); break;
  case SB_LE: printf("SB_LE\n"); break;
  case SB_GT: printf("SB_GT\n"); break;
  case SB_GE: printf("SB_GE\n"); break;
  case SB_PLUS: printf("SB_PLUS\n"); break;
  case SB_MINUS: printf("SB_MINUS\n"); break;
  case SB_TIMES: printf("SB_TIMES\n"); break;
  case SB_SLASH: printf("SB_SLASH\n"); break;
  case SB_LPAR: printf("SB_LPAR\n"); break;
  case SB_RPAR: printf("SB_RPAR\n"); break;
  case SB_LSEL: printf("SB_LSEL\n"); break;
  case SB_RSEL: printf("SB_RSEL\n"); break;

    // add +=, -=, *=, /=
  case SB_ASSIGN_PLUS: printf("SB_ASSIGN_PLUS\n");break;
  case SB_ASSIGN_SUBTRACT: printf("SB_ASSIGN_SUBTRACT\n");break;
  case SB_ASSIGN_TIMES: printf("SB_ASSIGN_TIMES\n");break;
  case SB_ASSIGN_DIVIDE: printf("SB_ASSIGN_DIVIDE\n");break;
  }
}

