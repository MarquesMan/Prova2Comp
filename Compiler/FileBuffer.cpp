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
//  OVERVIEW: FileBuffer.cpp
//  ========
//  Source file for file buffer.

#include "FileBuffer.h"

using namespace Compiler;


//////////////////////////////////////////////////////////
//
// FileBuffer implementation
// ==========
FileBuffer::FileBuffer(File& aFile):
  file(aFile)
//[]----------------------------------------------------[]
//|  Constructor                                         |
//[]----------------------------------------------------[]
{
  int size = file.length();
  char* source = new char[size + 1];

  size = file.read(source, size);
  source[size] = 0;
  setSource(source);
}

const char*
FileBuffer::getName() const
//[]----------------------------------------------------[]
//|  Get name                                            |
//[]----------------------------------------------------[]
{
  return file.getName();
}
