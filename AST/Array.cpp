#include "Array.h"

void AST::Array::write(Writer & write) const
{

	for (std::list<ObjectPtr<Value>>::const_iterator i = this->ValueList.begin(); i != this->ValueList.end(); ++i) {
		ObjectPtr<Value> test = *(i);
		test->write(write);
	}


}

void AST::Array::addTolist(ObjectPtr<Value> value) 
{
	this->ValueList.push_back(value);

}
