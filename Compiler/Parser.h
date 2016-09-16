#ifndef __Parser_h
#define __Parser_h

#include "Buffer.h"
#include "ErrorHandler.h"
#include "Token.h"
#include <list>
#include <map>
#include <utility>


namespace Compiler
{ // begin namespace Compiler

using namespace System;


//////////////////////////////////////////////////////////
//
// KeywordTableEntry: keyword table entry
// ==================
struct KeywordTableEntry
{
  const wchar_t* name;
  int token;
  void* value;

}; // KeywordTableEntry

//
// Auxiliary function
//
KeywordTableEntry* searchKeyword(KeywordTableEntry*, const wchar_t*);

//
// Macros to declare a keyword table
//
#define DECLARE_KEYWORD_TABLE(cls) \
private: \
  static KeywordTableEntry _keywords[]; \
protected: \
  virtual KeywordTableEntry* findKeyword(const wchar_t*)

#define DEFINE_KEYWORD_TABLE(cls) \
KeywordTableEntry* \
cls::findKeyword(const wchar_t* name) \
{ \
  return searchKeyword(_keywords, name); \
} \
DEFINE_KEYWORD_TABLE_ENTRIES(cls)

#define DEFINE_KEYWORD_TABLE2(cls, base) \
KeywordTableEntry* \
cls::findKeyword(const wchar_t* name) \
{ \
  KeywordTableEntry* k = searchKeyword(_keywords, name); \
  return k != 0 ? k : base::findKeyword(name); \
} \
DEFINE_KEYWORD_TABLE_ENTRIES(cls)

#define DEFINE_KEYWORD_TABLE_ENTRIES(cls) \
KeywordTableEntry cls::_keywords[] = {

#define KEYWORD(name, token, code) \
  {L##name, token, code}

#define END_KEYWORD_TABLE \
  {0, -1}}


//////////////////////////////////////////////////////////
//
// Parser: abstract parser class
// ======
class Parser: public ErrorHandler
{
public:
  // Destructor
  virtual ~Parser();

  void parse(const char*);

protected:
  enum // tokens
  {
    T_EOF = 0,			/* keyword EOF */
		T_TOKEN = 256,				/* keyword TOKEN */
		T_VOID,					/* keyword void */
		T_IDENTIFIER,
		T_INT,
		T_FLOAT,
		T_STRUCT,
    lastToken
  };

  //class Integer;
	class Function;
  class Keyword;
  enum // error codes
  {
    SYNTAX,
    UNEXPECTED_CHARACTER,
    EXPECTED,
    UNEXPECTED,
		NAME_EXPECTED,
		TOKEN_EXPECTED,
		EXPECTED_STRUCT,
    lastErrorCode
  };

  int lineNumber;
  ObjectPtr<Token> token;

  void deleteTokens();
  String getMessageFormat(const wchar_t*);

  Buffer& buffer()
  {
    return *input;
  }

  void setInput(Buffer* input)
  {
    this->input = input;
    lineNumber = 1;
    deleteTokens();
    token = nextToken();
  }

  virtual Token* nextToken();
  Token* lookahead(Token*, int = 1);

  void advance()
  {
    token = token->next != 0 ? token->next : nextToken();
  }

  void match(int);
	String matchIdentifier();
	//
	void matchStruct();

  DECLARE_KEYWORD_TABLE(Parser);
  DECLARE_ERROR_MESSAGE_TABLE(Parser);

  void start();
	
	struct StrcID{
		System::String Escopo;
		System::String Tipo;
		System::String ID;
		int Linha;
	};

	struct FunctionSTRC{
		System::String Escopo;
		System::String TipoRetorno;
		System::String ID;
		int Linha;
		std::list <StrcID> ListaParametros;
	};

	//Especificos da gramatica
	void CompilationUnit();
	void TypeDeclaration();
	void MembersOpt();
	void Member();
	void Declaration();
	void VariableDeclaration();
	void FunctionDeclaration();
	System::String Type();
	System::String PrimitiveType();
	System::String TypeName();
	System::String ReturnType();
	void ParametersOpt(struct FunctionSTRC *);
	void Parameter(struct FunctionSTRC *);

	//
	void printInfo();
	void VerificaReferencias();
	void VerificaRedefinicaoVAR();
	void VerificaRedefinicaoFOO();

	//Detecção de erros semânticos
	String Prefixo;
	
	//Tabela de tipos
	std::list<System::String> ListaTipos;
	
	//Tabela de variaves
	std::list <StrcID> ListaVariaveis;

	//Tabela de funções
	std::list <FunctionSTRC> ListaFuncoes;

private:
  ObjectPtr<Buffer> input;

}; // Parser


} // end namespace Compiler

#endif // __Parser_h
