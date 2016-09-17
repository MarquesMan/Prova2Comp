#ifndef __Node_h
#include "Node.h"
#endif

#ifndef __ValueNode_h
#include "ValueNode.h"
#endif

#ifndef __Value_h
#define __Value_h

using namespace AST;

class Value : public Node
{
public:
	void write(Writer&) const;

protected:
	ObjectPtr<ValueNode> ValueNodePointer;

};

#endif // Value_h