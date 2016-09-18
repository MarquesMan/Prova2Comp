
#ifndef __Node_h
#include "Value.h"
#endif

#ifndef __AST_Object_h
#define __AST_Object_h

namespace AST
{ // begin namespace AST

	class Object : public Value
	{
	public:
		void write(Writer&) const;

	protected:




	};

}

#endif // Value_h