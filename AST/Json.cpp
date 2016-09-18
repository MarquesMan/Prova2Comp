#include "Json.h"

#include "Value.h"

void Json::write(Writer & writer) const
{
	
	writer.beginBlock();
	if(this->Value!=NULL){
		this->Value->write(writer);
	}
	writer.endBlock();

}
