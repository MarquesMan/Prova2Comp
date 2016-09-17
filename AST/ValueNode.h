#ifndef __Node_h
#include "Node.h"
#endif

#ifndef __WString_h
#include "WString.h"
#endif

#ifndef __ValueNode_h
#define __ValueNode_h

using namespace AST;

class ValueNode : public Node
{
public:
	void write(Writer&) const;

protected:
	String lexeme;

};

#endif // Value_h