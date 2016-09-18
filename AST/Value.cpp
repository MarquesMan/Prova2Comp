#include "Value.h"

void Value::write(Writer & writer) const
{

	if (this->ValueNodePointer != NULL) {

			this->ValueNodePointer->write(writer);
	}
	
}
