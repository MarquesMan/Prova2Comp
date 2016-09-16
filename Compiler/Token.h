#ifndef __Token_h
#define __Token_h

//[]------------------------------------------------------------------------[]
//|                                                                          |
//|                        GVSG Foundation Classes                           |
//|                               Version 1.0                                |
//|                                                                          |
//|              Copyright® 2010-2015, Paulo Aristarco Pagliosa              |
//|              All Rights Reserved.                                        |
//|                                                                          |
//[]------------------------------------------------------------------------[]
//
//  OVERVIEW: Token.h
//  ========
//  Class definition for token.

#include "Object.h"
#include "WString.h"

namespace Compiler
{ // begin namespace Compiler

using namespace System;


//////////////////////////////////////////////////////////
//
// Token: token class
// =====
class Token: public Object
{
public:
  int type;
  int line;
  String lexeme;
  union
  {
    int integer;
    float real;
    int op;
    void* value;
  };

  // Contructors
  Token():
    type(-1),
    next(0)
  {
    // do nothing
  }

  Token(int aType):
    type(aType),
    next(0)
  {
    // do nothing
  }

  Token(int aType, const String& aLexeme):
    type(aType), lexeme(aLexeme),
    next(0)
  {
    // do nothing
  }

private:
  Token* next;

  friend class Parser;

}; // Token

} // end namespace Compiler

#endif // __Token_h
