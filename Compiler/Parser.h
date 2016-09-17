#ifndef __Parser_h
#define __Parser_h

#include "Buffer.h"
#include "ErrorHandler.h"
#include "Token.h"

#include <list>
#include <map>
#include <utility>
#include "AST\Json.h"


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

  ObjectPtr< Json > parse(const char*);
  void PrintInfo();
protected:
  enum // tokens
  {
    T_EOF = 0,
		T_TRUE = 256,
		T_FALSE,
		T_NULL,
		T_NUMBER,
    T_STRING,
    lastToken
  };

  class Integer;
  class Keyword;

  enum // error codes
  {
    SYNTAX,
    UNEXPECTED_CHARACTER,
    EXPECTED,
    UNEXPECTED,
    UNEXPECTED_CHAR_ON_STRING,
    UNEXPECTED_EOF,
    //
    // TODO: DECLARE SEUS CODIGOS DE ERROS AQUI
    //
    lastErrorCode
  };

  int lineNumber;
  inline int getLineNumber() { return lineNumber; }
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

  DECLARE_KEYWORD_TABLE(Parser);
  DECLARE_ERROR_MESSAGE_TABLE(Parser);

  void start(ObjectPtr< Json >);
  
  void _VALUE(ObjectPtr< Value >);
  void _VALUE(ObjectPtr< ValueNode >);

  void _OBJECT(ObjectPtr< ValueNode >);
  void _MEMBERS(ObjectPtr< ValueNode >);
  void _PAIR(ObjectPtr<ValueNode>);
  void _ARRAY(ObjectPtr< ValueNode >);
  void _ELEMENTS(ObjectPtr< ValueNode >);

  int num_Objetos, num_MembrosObjetos, num_Arrays, num_ElementosArrays;

private:
  ObjectPtr<Buffer> input;

}; // Parser

} // end namespace Compiler

#endif // __Parser_h
