#ifndef __ValueNode_h
#include "ValueNode.h"
#endif

#ifndef __Number_h
#define __Number_h

namespace AST
{ // begin namespace AST

	class Number : public ValueNode
	{
	public:
		void write(Writer&) const;
		Number(String lexeme) : ValueNode(lexeme) {};

	};

}

#endif // Value_h