#ifndef __ValueNode_h
#include "ValueNode.h"
#endif

#ifndef __True_h
#define __True_h

namespace AST
{ // begin namespace AST

	class True : public ValueNode
	{
	public:
		void write(Writer&) const;
		True() : ValueNode("True"){};

	};

}

#endif // Value_h