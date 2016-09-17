#ifndef __ValueNode_h
#include "ValueNode.h"
#endif

#ifndef __Pair_h
#include "Pair.h"
#endif

#ifndef __Members_h
#define __Members_h

#include <list>

namespace AST
{ // begin namespace AST

	class Members : public ValueNode
	{
	public:
		void write(Writer&) const;
		Members() : PairList(NULL) {};
		void addTolist(ObjectPtr<Pair>);

	protected:
		std::list<ObjectPtr<Pair>> PairList;

	};
}

#endif // Value_h