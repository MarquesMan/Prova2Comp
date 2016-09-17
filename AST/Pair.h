#ifndef __Value_h
#include "Value.h"
#endif

#ifndef __String_h
#define __String_h

using namespace AST;

class Pair : public Node
{
public:
	void write(Writer&) const;

protected:
	String identifier;
	ObjectPtr<Value> ValuePointer;

};

#endif // Value_h