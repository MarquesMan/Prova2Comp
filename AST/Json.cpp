#include "Json.h"



void Json::write(Writer & writer) const
{
	
	writer.write( L"{\n" );
	if(this->Value!=NULL)
		this->Value->write(writer);
	writer.write( L"}\n");

}
