#include "Parser.h"
#include "Writer.h"
#include "AST\Json.h"

using namespace System;

inline static void
usage()
{
  puts("Syntax is: jr <file>");
}

//
// Main function
//
int
main(int argc, char** argv)
{
  if (argc == 1)
    usage();
  else
    try
    {
      ObjectPtr< Json > json = Compiler::Parser().parse(argv[1]);
      Writer writer;
      json->write(writer);
	  getchar();
    }
    catch (const Exception& e)
    {
      printf("%ls", e.getMessage().c_str());
    }

  return 0;
}
