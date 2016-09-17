#ifndef __Json_h
#define __Json_h


#ifndef __Node_h
#include "Node.h"
#endif

#ifndef __Value_h
#include "Value.h"
#endif

using namespace AST;

class Json : public Node
{
public:
	void write(Writer&) const;
	Json() :Value(NULL) {};

protected:
	System::ObjectPtr<Value> Value;

	

};

#endif