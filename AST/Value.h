#ifndef __Value_h
#define __Value_h


#ifndef __Node_h
#include "Node.h"
#endif

#ifndef __ValueNode_h
#include "ValueNode.h"
#endif


namespace AST
{ // begin namespace AST

	class Value : public Node
	{
	public:
		void write(Writer&) const;

		void setValue(System::ObjectPtr<ValueNode> value) {
			this->ValueNodePointer = value;
		}

		Value() : ValueNodePointer(NULL) {};

	protected:
		ObjectPtr<ValueNode> ValueNodePointer;

	};

}

#endif // Value_h