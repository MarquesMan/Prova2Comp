#include "Json.h"



void Json::write(Writer & writer) const
{
	
	writer.write( (wchar_t*) "{\n" );
	if(this->Value!=NULL)
		this->Value->write(writer);
	writer.write((wchar_t*) "}\n");

}
