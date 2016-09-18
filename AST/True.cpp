#include "True.h"

void AST::True::write(Writer & write) const
{
	write.write(L"True");
}
