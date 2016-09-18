#include "Members.h"


void AST::Members::write(Writer & write) const
{

	for (std::list<ObjectPtr<Pair>>::const_iterator i = this->PairList.begin(); i != this->PairList.end(); ++i) {
		ObjectPtr<Pair> test = *(i);
		test->write(write);
		write.write(L",\n");
	}

}

void AST::Members::addTolist(ObjectPtr<Pair> pair)
{
	this->PairList.push_back(pair);

}
