#ifndef __Buffer_h
#define __Buffer_h

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
//  OVERVIEW: Buffer.h
//  ========
//  Class definition for generic input buffer.

#include "Object.h"
#include "WString.h"

namespace Compiler
{ // begin namespace Compiler

using namespace System;


//////////////////////////////////////////////////////////
//
// Buffer: generic input buffer class
// ======
class Buffer: public Object
{
public:
  virtual const char* getName() const = 0;

  void startLexeme()
  {
    mark = current;
  }

  String getLexeme() const
  {
    return String(mark, (int)(current - mark));
  }

  char lookahead() const
  {
    return *current;
  }

  char operator *() const
  {
    return lookahead();
  }

  char advance()
  {
    return *current ? *++current : 0;
  }

  char operator ++(int) // postfix
  {
    char temp = lookahead();

    advance();
    return temp;
  }

  char operator ++()
  {
    return advance();
  }

protected:
  char* start;
  char* current;
  char* mark;

  void setSource(char* source)
  {
    start = current = mark = source;
  }

}; // Buffer

} // end namespace Compiler

#endif // __Buffer_h
