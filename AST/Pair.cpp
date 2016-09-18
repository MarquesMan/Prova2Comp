#include "Pair.h"

void Pair::write(Writer & write) const
{

	write.write(this->identifier.c_str());
	write.write(L":");
	this->ValuePointer->write(write);
	

}
