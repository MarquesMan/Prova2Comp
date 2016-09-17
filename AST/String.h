#ifndef __ValueNode_h
#include "ValueNode.h"
#endif

#ifndef __String_AST_h
#define __String_AST_h

namespace AST
{ // begin namespace AST

	class String_AST : public ValueNode
	{
	public:
		void write(Writer&) const;
		String_AST(String lexeme) : ValueNode(lexeme) {};
	
	};

}

#endif // Value_h