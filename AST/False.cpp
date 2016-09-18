#include "False.h"

void AST::False::write(Writer & write) const
{

	write.write(L"False");

}
