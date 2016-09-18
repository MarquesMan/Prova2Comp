#include "Number.h"

void AST::Number::write(Writer & write) const
{

	write.write(this->lexeme.c_str());

}
