#ifndef __FileBuffer_h
#define __FileBuffer_h

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
//  OVERVIEW: FileBuffer.h
//  ========
//  Class definition for file buffer.

#include "Buffer.h"
#include "File.h"

namespace Compiler
{ // begin namespace Compiler

using namespace System;


//////////////////////////////////////////////////////////
//
// FileBuffer: file buffer class
// ==========
class FileBuffer: public Buffer
{
public:
  // Constructor
  FileBuffer(File&);

  // Destructor
  ~FileBuffer()
  {
    delete []start;
  }

  const File& getFile() const
  {
    return file;
  }

  const char* getName() const;

private:
  File& file;

}; // FileBuffer

} // end namespace Compiler

#endif // __FileBuffer_h
