/* Scanner
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __SCANNER_H__
#define __SCANNER_H__

#include "token.h"

void skipComment();
void skipBlank();
Token* getToken(void);
Token* getValidToken(void);
Token* readIdentKeyword(void);
Token* readNumber(void);
Token* readConstChar(void);
Token* readPlus();
Token* readMinus();
Token* readTimes();
Token* readSlash();

void printToken(Token *token);

#endif
