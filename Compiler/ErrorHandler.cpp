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
//  OVERVIEW: ErrorHandler.cpp
//  ========
//  Source file for generic error handler.

#include <stdio.h>
#ifndef WIN32
#include <wchar.h>
#endif

#include "ErrorHandler.h"

using namespace Compiler;

//
// Auxiliary function
//
const wchar_t*
Compiler::searchErrorMessage(ErrorMessageTableEntry* entries, int code)
//[]----------------------------------------------------[]
//|  Search error message                                |
//[]----------------------------------------------------[]
{
  for (; entries->code != -1; entries++)
    if (code == entries->code)
      return entries->message;
  return 0;
}


//////////////////////////////////////////////////////////
//
// ErrorHandler implementation
// ============
const wchar_t*
ErrorHandler::findMessage(int)
//[]----------------------------------------------------[]
//|  Find message                                        |
//[]----------------------------------------------------[]
{
  return 0;
}

void
ErrorHandler::error(int code, ...)
//[]----------------------------------------------------[]
//|  Error                                               |
//[]----------------------------------------------------[]
{
  va_list args;

  va_start(args, code);
  handle(code, args);
}

const wchar_t*
ErrorHandler::getMessage(int code)
//[]----------------------------------------------------[]
//|  Get message                                         |
//[]----------------------------------------------------[]
{
  const wchar_t* msg = findMessage(code);
  return msg == 0 ? L"undefined error code" : msg;
}

#define EM_MAXLEN 1023
#define EF_MAXLEN 1023

void
ErrorHandler::error(const wchar_t* fmt, ...)
//[]----------------------------------------------------[]
//|  Error                                               |
//[]----------------------------------------------------[]
{
  va_list args;
  wchar_t temp[EM_MAXLEN + 1];

  va_start(args, fmt);
  vswprintf(temp, EM_MAXLEN, fmt, args);
  // throw error
  throwError(temp);
}

void
ErrorHandler::handle(int code, va_list args)
//[]----------------------------------------------------[]
//|  Handle                                              |
//[]----------------------------------------------------[]
{
  wchar_t temp[EM_MAXLEN + 1];
  String fmt = getMessageFormat(getMessage(code));

  vswprintf(temp, EM_MAXLEN, fmt, args);
  // throw error
  throwError(temp);
}

String
ErrorHandler::getMessageFormat(const wchar_t* msg)
//[]----------------------------------------------------[]
//|  Get message format                                  |
//[]----------------------------------------------------[]
{
  wchar_t fmt[EF_MAXLEN + 1];

  swprintf(fmt, EF_MAXLEN, L"Error: %ls\n", msg);
  return String(fmt);
}

void
ErrorHandler::throwError(const String& msg)
//[]----------------------------------------------------[]
//|  Throw error                                         |
//[]----------------------------------------------------[]
{
  throw Exception(msg);
}
