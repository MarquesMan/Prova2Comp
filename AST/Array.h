#ifndef __ValueNode_h
#include "ValueNode.h"
#endif

#ifndef __Value_h
#include "Value.h"
#endif

#ifndef __Array_h
#define __Array_h

#include <list>

namespace AST
{ // begin namespace AST

	class Array : public ValueNode
	{
	public:
		void write(Writer&) const;
		Array() : ValueList(NULL) {};
		void addTolist(ObjectPtr<Value>);
	protected:
		std::list<ObjectPtr<Value>> ValueList;

	};

}
#endif // Value_h

