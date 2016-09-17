#include "Members.h"


void AST::Members::write(Writer &) const
{
}

void AST::Members::addTolist(ObjectPtr<Pair> pair)
{
	this->PairList.push_back(pair);

}
