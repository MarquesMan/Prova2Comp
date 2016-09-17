#include "Json.h"

#include "Value.h"

void Json::write(Writer & writer) const
{
	
	writer.write( L"{\n" );
	if(this->Value!=NULL){
		writer.beginBlock();
		this->Value->write(writer);
		writer.endBlock();
	}
	writer.write( L"}\n");

}
