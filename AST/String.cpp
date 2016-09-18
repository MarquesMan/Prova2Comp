#include "String.h"

void AST::String_AST::write(Writer & write) const
{

	write.write(this->lexeme.c_str());

}
