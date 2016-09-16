#ifndef __Node_h
#define __Node_h

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
//  OVERVIEW: Node.h
//  ========
//  Class definition for generic AST node.

#ifndef __Object_h
#include "./System/Object.h"
#endif
#ifndef __Writer_h
#include "Writer.h"
#endif

namespace AST
{ // begin namespace AST

	using namespace System;


	//////////////////////////////////////////////////////////
	//
	// Node: generic AST node class
	// ====
	class Node : public Object
	{
	public:
		virtual void write(Writer&) const = 0;

	}; // Node

} // end namespace AST

#endif // __Node_h
