#ifndef __ValueNode_h
#include "ValueNode.h"
#endif

#ifndef __Value_h
#include "Value.h"
#endif

#ifndef __Members_h
#define __Members_h

using namespace AST;

class Array : public ValueNode
{
public:
	void write(Writer&) const;

protected:
	template<Value> ValueList list;

};

#endif // Value_h