#ifndef __ValueNode_h
#include "ValueNode.h"
#endif

#ifndef __False_h
#define __False_h

namespace AST
{ // begin namespace AST

	class False : public ValueNode
	{
	public:
		void write(Writer&) const;
		False() : ValueNode("False") {};

	};

}

#endif // Value_h