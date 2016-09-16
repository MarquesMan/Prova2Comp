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
//  OVERVIEW: Object.cpp
//  ========
//  Source file for generic object.

#include "Object.h"

using namespace System;


//////////////////////////////////////////////////////////
//
// Object implementation
// ======
Object::~Object()
//[]---------------------------------------------------[]
//|  Destructor                                         |
//[]---------------------------------------------------[]
{
  // do nothing
}

Object*
Object::clone() const
//[]---------------------------------------------------[]
//| Make a deep-copy of this object                     |
//[]---------------------------------------------------[]
{
  return 0;
}
