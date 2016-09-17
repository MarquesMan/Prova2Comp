#include "Array.h"

void AST::Array::write(Writer &) const
{
}

void AST::Array::addTolist(ObjectPtr<Value> value) 
{
	this->ValueList.push_back(value);

}
