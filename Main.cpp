#include "Parser.h"

using namespace System;

inline static void
usage()
{
  puts("Syntax is: p1 <file>");
}

//
// Main function
//
int
main(int argc, char** argv)
{
  puts("comp2015-p1, Compiladores 2015. FACOM-UFMS");
  if (argc == 1)
    usage();
  else
    try
    {
      Compiler::Parser().parse(argv[1]);
			//
    }
    catch (const Exception& e)
    {
      printf("%ls", e.getMessage().c_str());
    }
  puts("Press any key to exit...");
  getchar();
  return 0;
}
