#ifndef __ValueNode_h
#include "ValueNode.h"
#endif

#ifndef __String_h
#define __String_h

using namespace AST;

class String : public ValueNode
{
public:
	void write(Writer&) const;
	

};

#endif // Value_h