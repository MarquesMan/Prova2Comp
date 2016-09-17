#ifndef __ValueNode_h
#include "ValueNode.h"
#endif

#ifndef __Pair_h
#include "Pair.h"
#endif

#ifndef __Members_h
#define __Members_h

using namespace AST;

class Members : public ValueNode
{
public:
	void write(Writer&) const;

protected:
	template<Pair> PairList list;

};

#endif // Value_h