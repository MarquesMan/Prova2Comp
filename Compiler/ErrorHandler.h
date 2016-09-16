#ifndef __ErrorHandler_h
#define __ErrorHandler_h

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
//  OVERVIEW: ErrorHandler.h
//  ========
//  Class definition for generic error handler.

#include <stdarg.h>
#include "Exception.h"

namespace Compiler
{ // begin namespace Compiler

using namespace System;


//////////////////////////////////////////////////////////
//
// ErrorMessageTableEntry: error message table entry
// ======================
class ErrorMessageTableEntry
{
public:
  int code;
  const wchar_t* message;

}; // ErrorMessageTableEntry

//
// Auxiliary function
//
const wchar_t* searchErrorMessage(ErrorMessageTableEntry*, int);

//
// Macros to declare a error message table
//
#define DECLARE_ERROR_MESSAGE_TABLE(cls) \
private: \
  static Compiler::ErrorMessageTableEntry _errmsg[]; \
protected: \
  const wchar_t* findMessage(int)

#define DEFINE_ERROR_MESSAGE_TABLE(cls, base) \
const wchar_t* \
cls::findMessage(int code) \
{ \
  const wchar_t* msg = Compiler::searchErrorMessage(_errmsg, code); \
  if (msg == 0) \
    msg = base::findMessage(code); \
  return msg; \
} \
DEFINE_ERROR_MESSAGE_TABLE_ENTRIES(cls)

#define DEFINE_ERROR_MESSAGE_TABLE_ENTRIES(cls) \
Compiler::ErrorMessageTableEntry cls::_errmsg[] = {

#define ERROR_MESSAGE(code, msg) \
{code, L##msg}

#define END_ERROR_MESSAGE_TABLE \
{-1, 0}}


//////////////////////////////////////////////////////////
//
// ErrorHandler: generic error handler class
// ============
class ErrorHandler
{
public:
  void error(int, ...);
  void error(const wchar_t*, ...);

protected:
  virtual const wchar_t* findMessage(int);
  virtual String getMessageFormat(const wchar_t*);
  virtual void handle(int, va_list);
  virtual void throwError(const String&);

  const wchar_t* getMessage(int);

}; // ErrorHandler

} // end namespace Compiler

#endif // __ErrorHandler_h
