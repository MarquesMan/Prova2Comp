#include "Json.h"

void Json::write(Writer & writer) const
{
	printf("Json:\n");
	this->Value->write(writer);
	printf("------------------------------------\nFIM\n");


}
