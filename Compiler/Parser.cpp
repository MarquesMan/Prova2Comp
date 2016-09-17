#include <ctype.h>
#include <stdlib.h>
#include <iostream>


#include "FileBuffer.h"
#include "AST\Value.h"
#include "AST\ValueNode.h"

#include "AST\Object.h"
#include "AST\Array.h"
#include "AST\False.h"
#include "AST\Null.h"
#include "AST\String.h"
#include "AST\Pair.h"
#include "AST\True.h"
#include "AST\Number.h"

#include "AST\Members.h"


#include "Parser.h"

using namespace Compiler;

//
// Auxiliary function
//
KeywordTableEntry*
Compiler::searchKeyword(KeywordTableEntry* k, const wchar_t* name)
//[]----------------------------------------------------[]
//|  Search keyword                                      |
//[]----------------------------------------------------[]
{
  for (; k->name != 0; k++)
    if (wcscmp(k->name, name) == 0)
      return k;
  return 0;
}


//////////////////////////////////////////////////////////
//
// Parser::Keyword: keyword token class
// ===============
class Parser::Keyword: public Token
{
public:
  // Constructor
  Keyword(const KeywordTableEntry* k):
    Token(k->token, k->name)
  {
    this->value = k->value;
  }

}; // Parser::Keyword


//////////////////////////////////////////////////////////
//
// Parser implementation
// ======
DEFINE_KEYWORD_TABLE(Parser)
	KEYWORD("true", T_TRUE, 0),
	KEYWORD("false", T_FALSE, 0),
	KEYWORD("null", T_NULL, 0),
END_KEYWORD_TABLE;

DEFINE_ERROR_MESSAGE_TABLE(Parser, ErrorHandler)
  ERROR_MESSAGE(SYNTAX,
    "Syntax error"),
  ERROR_MESSAGE(UNEXPECTED_CHARACTER,
    "Unexpected character '%C'"),
  ERROR_MESSAGE(EXPECTED,
    "Expected '%C'"),
  ERROR_MESSAGE(UNEXPECTED,
    "Unexpected '%ls'"),
  ERROR_MESSAGE(UNEXPECTED_CHAR_ON_STRING,
    "Unexpected char inside string definition '%c'"),
  ERROR_MESSAGE(UNEXPECTED_EOF,
    "Unexpected EOF inside string definition"),
  //
  // TODO: DEFINA SUAS MENSAGENS DE ERRO AQUI
  //
END_ERROR_MESSAGE_TABLE;

ObjectPtr< Json >
Parser::parse(const char* fileName)
{
  File file;
  ObjectPtr< Json > json;

  if (!file.open(fileName, File::readOnly))
    error(L"Nao foi possivel encontrar o arquivo '%S'\n", fileName);

  FileBuffer* input = new FileBuffer(file);

  setInput(input);
  file.close();

  json = new Json();

  start(json);

  return json;

}

void Compiler::Parser::PrintInfo()
{
  std::cout << "Objetos: \t" << num_Objetos << "\nMembros: \t" << num_MembrosObjetos << "\nArrays: \t" << num_Arrays << "\nElementos: \t" << num_ElementosArrays << std::endl;
}

Parser::~Parser()
//[]----------------------------------------------------[]
//|  Destructor                                          |
//[]----------------------------------------------------[]
{
  deleteTokens();
}

void
Parser::deleteTokens()
//[]----------------------------------------------------[]
//|  Delete tokens                                       |
//[]----------------------------------------------------[]
{
  for (Token* t; token != 0; t = token->next, token = t)
    ;
}

#define EF_MAXLEN 1023
#define EF_FORMAT L"error %ls %d: %ls\n"

String
Parser::getMessageFormat(const wchar_t* msg)
//[]----------------------------------------------------[]
//|  Get message format                                  |
//[]----------------------------------------------------[]
{
  wchar_t fmt[EF_MAXLEN + 1];

  swprintf(fmt, EF_MAXLEN, EF_FORMAT, input->getName(), lineNumber, msg);
  return String(fmt);
}

#undef EF_MAXLEN
#undef EF_FORMAT

Token*
Parser::nextToken()
//[]----------------------------------------------------[]
//|  Next token                                          |
//[]----------------------------------------------------[]
{
_space:
  while (isspace(*buffer()))
    if (buffer()++ == '\n')
      lineNumber++;
  if (*buffer() == 0)
    return new Token(T_EOF);
	
  //Inicia lexeme 
  buffer().startLexeme();
  
  //String
  if (*buffer() == '\"') {
    int c = ++buffer();

    //String vazia
    if (c == '\"') {
      String lexeme = buffer().getLexeme();
      return new Token(T_STRING, lexeme);
    }

    //Varre a string
    while (c != '\"') {
      if (c == EOF)
        error(UNEXPECTED_EOF);
      
      //Se for algum dos char proibidos, retorne erro
      if (c == '\t' || c == '\n')
        error(UNEXPECTED_CHAR_ON_STRING, c);

      //Caso seja uma barra invertida
      if (c == '\\') {
        c = ++buffer();
        switch (c) {
          case '\"':
          case '\\':
          case 't':
          case 'n':
            break;
          default:
            error(UNEXPECTED_CHAR_ON_STRING, c);
        }
      }
      //Caso seja qualquer outro char ascii, ta tudo certo. Pode continuar no loop até encontrar o fim da string
      //Avança para char seguinte
      c = ++buffer();
    }
    ++buffer();// Por algum motivo que eu não entendi, eu preciso avançar o buffer aqui se não ele ignora a ultima aspas dupla (?????)
    
    //Retorna o token com o lexeme da string
    String lexeme = buffer().getLexeme();
    return new Token(T_STRING, lexeme);
  }
  
  //Number
  if (*buffer() == '-' || (*buffer() >= '0' && *buffer() <= '9')) {
    //Se começar com '-'
    if (*buffer() == '-')
      ++buffer();

    //Se o Number começar com 0, verifique se o proximo char não é um digito.
    if (*buffer() == '0') {
      ++buffer();

      //É um digito
      if (*buffer() >= '0' && *buffer() <= '9') {
        int c = *buffer();
        error(UNEXPECTED_CHARACTER, c);
      }

      //Não é um digito.
      if (*buffer() == '.')
        goto _FRACIONARIA;
      if (*buffer() == 'e' || *buffer() == 'E')
        goto _EXPONENCIAL;
      //apenas um digito 0
      return new Token(T_NUMBER, buffer().getLexeme());
    }

    int c = *buffer();
    //Avança enquanto forem digitos
    do {
      c = ++buffer();
    } while (c >= '0' && c <= '9');
      
    //Fração
    if (c == '.') {
    _FRACIONARIA:
      c = ++buffer();

      //Se houverem digitos
      if (c >= '0' && c <= '9') {
        //Avança enquanto forem digitos
        do {
          c = ++buffer();
        } while (c >= '0' && c <= '9');

        //Acabaram os digitos. Mas verifique se existe uma parte exponencial
        if (c == 'e' || c == 'E')
          goto _EXPONENCIAL;
        //Não tem parte exponencial. Então o numero foi finalizado
        return new Token(T_NUMBER, buffer().getLexeme());
      }
      //Caso contrario, erro de sintaxe
      else
        error(UNEXPECTED_CHARACTER, c);
    }

    //Exponencial
    if (c == 'e' || c == 'E') {
    _EXPONENCIAL:
      c = ++buffer();
      if (c == '-' || c == '+') {
        c = ++buffer();
      }
      if (c >= '0' && c <= '9') {
        //Avança enquanto forem digitos
        do {
          c = ++buffer();
        } while (c >= '0' && c <= '9');
        return new Token(T_NUMBER, buffer().getLexeme());
      }
      //Caso contrario, erro de sintaxe
      else
        error(UNEXPECTED_CHARACTER, c);
    }

    //Se não tiver parte fracionaria nem parte exponencial, é apenas um numero
     return new Token(T_NUMBER, buffer().getLexeme());
  }

  //Palavra 'reservada'
  if (isalpha(*buffer()))
  {
    while (isalnum(++buffer()));

    String lexeme = buffer().getLexeme();
    KeywordTableEntry* k = findKeyword(lexeme);
    //Se não for nenhuma das palavras reservadas, retorne erro de sintaxe
    if (k == 0)
      error(SYNTAX);
    return new Keyword(k);
  }

  int c = buffer()++;
  //Tokens de char unico
  switch (c)
  {
  case '{':
  case '}':
  case '[':
  case ']':
  case ',':
  case ':':
    break;
  default:
    error(UNEXPECTED_CHARACTER, c);
  }
  return new Token(c, buffer().getLexeme());
}

Token*
Parser::lookahead(Token* token, int n)
//[]----------------------------------------------------[]
//|  Get token                                           |
//[]----------------------------------------------------[]
{
  Token* t = token;

  for (int i = 0; i < n; i++)
    t = t->next != 0 ? t->next : (t->next = nextToken());
  return t;
}

void
Parser::match(int c)
//[]----------------------------------------------------[]
//|  match                                               |
//[]----------------------------------------------------[]
{
  if (token->type != c)
  {
    if (token->type == T_EOF)
      error(EXPECTED, c);
    error(UNEXPECTED, token->lexeme.c_str());
  }
  advance();
}

void
Parser::start(ObjectPtr< Json > json)
//[]----------------------------------------------------[]
//|	start																								 |
//[]----------------------------------------------------[]
{
  num_Objetos = num_MembrosObjetos = num_Arrays = num_ElementosArrays = 0;
  
  ObjectPtr<Value> Valor = new AST::Value(); // Instancia Value
  json->setValue(Valor);// Adiciona Value a Json
  
  _VALUE(Valor);
  PrintInfo();
}

void Compiler::Parser::_VALUE(ObjectPtr< Value > Value)
{


  //Object
  if (token->type == '{') {
	ObjectPtr< ValueNode > Object = new AST::Members();
	Value->setValue(Object);
    _OBJECT(Object);
    return;
  }

  //Array
  if (token->type == '[') {
	ObjectPtr< ValueNode > Array = new AST::Array();
	Value->setValue(Array);
    _ARRAY(Array);
    return;
  }

  //String, Number, True, False, Null
  if (token->type == T_STRING || token->type == T_NUMBER || token->type == T_TRUE || token->type == T_FALSE || token->type == T_NULL) {

	  if (token->type == T_STRING) {
		  ObjectPtr< ValueNode > String = new AST::String_AST(token->lexeme);
		  Value->setValue(String);
	  }
	  else if (token->type == T_NUMBER) {
		  ObjectPtr< ValueNode > Number = new AST::Number(token->lexeme);
		  Value->setValue(Number);
	  
	  }
	  else if (token->type == T_TRUE) {
		  ObjectPtr< ValueNode > Number = new AST::True();
		  Value->setValue(Number);
	  }
	  else if (token->type == T_FALSE) {
		  ObjectPtr< ValueNode > False = new AST::False();
		  Value->setValue(False);
	  }
	  else if (token->type == T_NULL) {
		  ObjectPtr< ValueNode > Number = new AST::Null();
		  Value->setValue(Number);
	  }


    advance();
    return;
  }
  //Se não for nada disso, erro
  std::wcout << token->type << '\n';
  error(UNEXPECTED_CHARACTER, token->type);
}


void Compiler::Parser::_OBJECT(ObjectPtr< ValueNode > Object)
{
  num_Objetos++;
  match('{');
  //se ja for o final do objeto, retorne
  if (token->type == '}') {
    match('}');
    return;
  }
  //Entao o objeto não é vazio. Chame membros
  _MEMBERS(Object);

  //Assim que retornar de membros, de match no '}'
  match('}');
}

void Compiler::Parser::_MEMBERS(ObjectPtr< ValueNode > Object)
{
  num_MembrosObjetos++;
  _PAIR(Object);
  while (token->type == ',') {
    advance();
    num_MembrosObjetos++;
    _PAIR(Object);
  }
}

void Compiler::Parser::_PAIR(ObjectPtr<ValueNode> members)
{
	
	ObjectPtr<Pair> par = new Pair(token->lexeme);	// Gera o par e a String de id dele 
	match(T_STRING);
	match(':');
	
	Members* Member = dynamic_cast<Members*>((ValueNode*)members); // cast para usar o members como uma variavel Members	
	Member->addTolist(par);// Adiciona o par criado ao members

	ObjectPtr<Value> parValue = new Value();// Cria um Value para o par
	par->setValuePointer(parValue);// seta o Value para o par

	_VALUE(parValue);// entramos com ele para um novo nivel de recursao 

}

void Compiler::Parser::_ARRAY(ObjectPtr<ValueNode> Array)
{
  
	num_Arrays++;
  match('[');

  if (token->type == T_STRING || token->type == T_NUMBER || token->type == T_TRUE || token->type == T_FALSE || token->type == T_NULL || token->type == '{' || token->type == '[') {
    _ELEMENTS(Array);
  }
  match(']');
}

void Compiler::Parser::_ELEMENTS(ObjectPtr<ValueNode> Array)
{

  num_ElementosArrays++;

  ObjectPtr<Value> oneValue = new Value();// Cria um Value para o par

  AST::Array* currentArray = dynamic_cast<AST::Array*>((ValueNode*)Array); // cast para usar o members como uma variavel Members	
  currentArray->addTolist(oneValue);// Adiciona o par criado ao members

  _VALUE(oneValue);

  while (token->type == ',') {

	advance();
    num_ElementosArrays++;

	oneValue = new Value();// Cria mais um Value para o par
	currentArray->addTolist(oneValue);// Adiciona o par criado ao members

    _VALUE(oneValue);
  
  }
}
