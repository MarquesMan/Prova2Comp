#include "Null.h"

void AST::Null::write(Writer & write) const
{

	write.write(L"Null");

}
