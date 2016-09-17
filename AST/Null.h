#ifndef __ValueNode_h
#include "ValueNode.h"
#endif

#ifndef __Null_h
#define __Null_h

namespace AST
{ // begin namespace AST

	class Null : public ValueNode
	{
	public:
		void write(Writer&) const;
		Null() : ValueNode("Null") {};

	};
}

#endif // Value_h