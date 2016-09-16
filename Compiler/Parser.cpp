#include <ctype.h>
#include <stdlib.h>
#include <iostream>

#include "FileBuffer.h"
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


//////////////////////////////////////////////////////////TODO
//
// Parser implementation
// ======
DEFINE_KEYWORD_TABLE(Parser)
	KEYWORD("void", T_VOID, 0),
	KEYWORD("struct", T_STRUCT, 0),
	KEYWORD("int", T_INT, 0),
	KEYWORD("float", T_FLOAT, 0),
	KEYWORD("Indentifier", T_IDENTIFIER, 0),
END_KEYWORD_TABLE;

DEFINE_ERROR_MESSAGE_TABLE(Parser, ErrorHandler)
	ERROR_MESSAGE(EXPECTED_STRUCT,
	"Expected Struct"),
  ERROR_MESSAGE(SYNTAX,
    "Syntax error"),
  ERROR_MESSAGE(UNEXPECTED_CHARACTER,
    "Unexpected character '%C'"),
  ERROR_MESSAGE(EXPECTED,
    "Expected '%C'"),
  ERROR_MESSAGE(UNEXPECTED,
    "Unexpected '%ls'"),
	ERROR_MESSAGE(NAME_EXPECTED,
	"Unexpected '%ls'"),
	ERROR_MESSAGE(TOKEN_EXPECTED,
	"Expected Token"),
  //
  // TODO: DEFINA SUAS MENSAGENS DE ERRO AQUI
  //
END_ERROR_MESSAGE_TABLE;

void
Parser::parse(const char* fileName)
{
  File file;

  if (!file.open(fileName, File::readOnly))
    error(L"error: Could not find module file '%S'\n", fileName);

  FileBuffer* input = new FileBuffer(file);

  setInput(input);
  file.close();
  start();
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

	//indentificador
	buffer().startLexeme();
	if (isalpha(*buffer()) || *buffer() == '_')
	{
		while (isalnum(++buffer()) || *buffer() == '_')
			;

		String lexeme = buffer().getLexeme();
		KeywordTableEntry* k = findKeyword(lexeme);//verifica se eh uma palavra reservada

		return k == 0 ? new Token(T_IDENTIFIER, lexeme) : new Keyword(k);
	}


	int c = buffer()++;
	
	switch (c)
	{
		case '{':
		case '}':
		case '(':
		case ')':
		case ',':
		case '.':
		case ';':
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
	int value = token->type;

  if (token->type != c)
  {
    if (token->type == T_EOF)
      error(EXPECTED, c);
    error(UNEXPECTED, token->lexeme.c_str());
  }
  advance();
}

String
Parser::matchIdentifier()
//[]----------------------------------------------------[]
//|  matchIdentifier                                     |
//[]----------------------------------------------------[]
{
	if (token->type != T_IDENTIFIER){
		error(NAME_EXPECTED);
	}
	String identifier = token->lexeme;

	advance();
	return identifier;
}

void Parser::matchStruct(){
	if (token->type == T_STRUCT)
	{
		advance();
	}
	else
		error(EXPECTED_STRUCT);
}

void
Parser::printInfo(){
	//Imprime os tipos declarados
	std::wcout << "Lista de tipos definidos:\n";
	for (std::list<System::String>::iterator i = this->ListaTipos.begin(); i != this->ListaTipos.end(); i++)
		std::wcout << "     " << (*i).c_str() << "\n";


	std::wcout << "\nLista de variaveis declarados:\n";
	//Imprime as variaves declaradas. Junto com seus tipos;
	for (std::list<StrcID>::iterator i = this->ListaVariaveis.begin(); i != this->ListaVariaveis.end(); i++)
		std::wcout << (*i).Linha << ":\t" << (*i).Escopo.c_str() << " " << (*i).Tipo.c_str() << " " << (*i).ID.c_str()  << "\n";


	std::wcout << "\nLista de funcoes e seus parametros:\n";
	//Imprime as funcoes junto com suas listas de parametros
	for (std::list<FunctionSTRC>::iterator i = this->ListaFuncoes.begin(); i != this->ListaFuncoes.end(); i++){
		std::wcout << (*i).Linha << ":\t" << (*i).Escopo.c_str() << " " << (*i).TipoRetorno.c_str() << " " << (*i).ID.c_str() << "(\n";

		for (std::list<StrcID>::iterator i2 = (*i).ListaParametros.begin(); i2 != (*i).ListaParametros.end(); i2++)
			std::wcout << "\t" << (*i2).Tipo.c_str() << " " << (*i2).ID.c_str() << "\n";
		std::wcout << ")\n";

	}
}

void
Parser::VerificaReferencias(){
	//para cada variavel na lista de variaves verificar se existe um tipo correspondente
	for (std::list<StrcID>::iterator i = this->ListaVariaveis.begin(); i != this->ListaVariaveis.end(); i++){
		System::String Tipo = (*i).Tipo;
		System::String Escopo = (*i).Escopo;
		bool VariavelOK = false;

		for (std::list<System::String>::iterator i2 = this->ListaTipos.begin(); i2 != this->ListaTipos.end(); i2++){
			//Dentro do escopo da variavel
			if (Tipo == "float" || Tipo == "int")
				VariavelOK = true;
			if (Escopo + Tipo == (*i2)){//ok
				VariavelOK = true;
				break;
			}
			//Verifica se tipo existe no escopo global
			if (Tipo == (*i2)){//ok
				VariavelOK = true;
				break;
			}
		}
		//Tiponão encontrado
		if (!VariavelOK)
			std::wcout << "Referencia a tipo \"" << Tipo.c_str() << "\" indefinido na linha " << (*i).Linha <<".\n";
	}
	//Para cada tipo de retorno
	for (std::list<FunctionSTRC>::iterator i = this->ListaFuncoes.begin(); i != this->ListaFuncoes.end(); i++){
		System::String Tipo = (*i).TipoRetorno;
		System::String Escopo = (*i).Escopo;
		bool FooOk = false;
		
		if (Tipo == "void" || Tipo == "float" || Tipo == "int")
			FooOk = true;
		else{
			for (std::list<System::String>::iterator i2 = this->ListaTipos.begin(); i2 != this->ListaTipos.end(); i2++){
				//Dentro do escopo da variavel
				if (Escopo + Tipo == (*i2)){//ok
					FooOk = true;
					break;
				}
				//Verifica se tipo existe no escopo global
				if (Tipo == (*i2)){//ok
					FooOk = true;
					break;
				}
			}
		}
		if (!FooOk)
			std::wcout << "Referencia a tipo \"" << (*i).TipoRetorno.c_str() << "\" indefinido na linha " << (*i).Linha << " em definição de função.\n";
		FooOk = false;
		//Parametros
		for (std::list<StrcID>::iterator i2 = (*i).ListaParametros.begin(); i2 != (*i).ListaParametros.end(); i2++){
			Tipo = (*i2).Tipo;
			
			if (Tipo == "float" || Tipo == "int")
				FooOk = true;
			for (std::list<System::String>::iterator i2 = this->ListaTipos.begin(); i2 != this->ListaTipos.end(); i2++){
				//Dentro do escopo da variavel
				if (Escopo + Tipo == (*i2)){//ok
					FooOk = true;
					break;
				}
				//Verifica se tipo existe no escopo global
				if (Tipo == (*i2)){//ok
					FooOk = true;
					break;
				}
			}
			if (!FooOk)
				std::wcout << "Referencia a tipo \"" << Tipo.c_str() << "\" indefinido na linha " << (*i2).Linha << " em definição de função.\n";
		}
	}
}

void
Parser::VerificaRedefinicaoVAR(){
	//para cada var
	for (std::list<StrcID>::iterator i = this->ListaVariaveis.begin(); i != this->ListaVariaveis.end(); i++)
	{
		for (std::list<StrcID>::iterator i2 = this->ListaVariaveis.begin(); i2 != this->ListaVariaveis.end(); i2++)
		{
			if ((*i).Escopo == (*i2).Escopo && (*i).ID == (*i2).ID && (*i).Linha != (*i2).Linha)
				std::wcout << "Redefinição de variavel nas linhas " << (*i).Linha << " e " << (*i2).Linha << "\n";
		}
	}
	//Para cada tipo
	for (std::list<System::String>::iterator i = this->ListaTipos.begin(); i != this->ListaTipos.end(); i++){
		for (std::list<System::String>::iterator i2 = this->ListaTipos.begin(); i2 != this->ListaTipos.end(); i2++){
			if ((*i) == (*i2))
				std::wcout << "Redefinição de tipo \"" << (*i).c_str() << "\".\n";
		}
	}
}

void
Parser::start()
//[]----------------------------------------------------[]
//|	start																								 |
//[]----------------------------------------------------[]
{
	this->Prefixo = "";
	CompilationUnit();
	this->printInfo();

	//Se as declarações de variaveis e funcoes são de tipos existentes.
	this->VerificaReferencias();

	//Se variaveis no mesmo escopo possuem IDs diferentes.
	this->VerificaRedefinicaoVAR();

	//Se funções com mesmo ID possuem parametros diferentes
	//this->VerificaRedefinicaoFOO();
}


void Parser::CompilationUnit()
{
	while (token->type != T_EOF)
		TypeDeclaration();
}
void Parser::TypeDeclaration()
{
	//Salva prefixo
	String LastPrefixo = this->Prefixo;
	matchStruct();

	//Recupera ID do novo tipo
	String ID_Tipo = matchIdentifier();

	this->ListaTipos.push_back(this->Prefixo + ID_Tipo);

	//Atualiza prefixo
	this->Prefixo = this->Prefixo + ID_Tipo + ".";

	match('{');
	MembersOpt();
	match('}');

	//Retorna prefixo anterior
	this->Prefixo = LastPrefixo;
}
void Parser::MembersOpt(){
	while (token->type != '}')
		Member();

}
void Parser::Member(){

	if (token->type == T_STRUCT)
		TypeDeclaration();
	else
		Declaration();
}
void Parser::Declaration(){
	//Variable: (	      INT | FLOAT | TypeName) INDENTIFIER ';'
	//Function: (VOID | INT | FLOAT | TypeName) INDENTIFIER '('
	//verifica qual caractere aparece primeiro '(' ou ';'.
	auto T = lookahead(token, 1);
	for (; T->type != ';' && T->type != '('; T = lookahead(T, 1));

	if (T->type == ';')
		VariableDeclaration();
	else
		FunctionDeclaration();
}
void Parser::VariableDeclaration(){

	struct StrcID Var1;
	
	Var1.Tipo		= Type();
	Var1.ID			= matchIdentifier();
	Var1.Escopo = this->Prefixo;
	Var1.Linha	= lineNumber;
	match(';');

	//Insere na tabela
	this->ListaVariaveis.push_back(Var1);

	//Verifica se tipo existe na 

}
void Parser::FunctionDeclaration(){
	struct FunctionSTRC Var1;
	Var1.TipoRetorno	= ReturnType();
	Var1.ID						= matchIdentifier();
	Var1.Escopo				= this->Prefixo;
	Var1.Linha				= this->lineNumber;

	//Altera o escopo/prefixo
	this->Prefixo = this->Prefixo + "." + Var1.ID + "_";

	match('(');
	ParametersOpt(&Var1);
	match(')');
	match(';');

	//retorna o prefixo/escopo correto
	this->Prefixo = Var1.Escopo;
	this->ListaFuncoes.push_back(Var1);
}
System::String Parser::Type(){
	//Retorna uma string contendo o nome do tipo da variavel
	if (token->type == T_INT || token->type == T_FLOAT)
		return PrimitiveType();
	else
		return TypeName();
}
System::String Parser::PrimitiveType(){
	//Retorna uma string contendo o nome do tipo da variavel
	System::String tipo = token->lexeme;
	if (token->type == T_INT)
		tipo = "int";
	else
		tipo = "float";
	advance();
	return tipo;
}
System::String Parser::TypeName(){
	//Retorna uma string contendo o nome do tipo da variavel
	System::String tipo;

	tipo += matchIdentifier();
	while (token->type == '.'){
		match('.');
		tipo = tipo + "." + matchIdentifier();
	}
	return tipo;
}
System::String Parser::ReturnType(){
	//Retorna uma string contendo o nome do tipo da variavel
	if (token->type == T_VOID)
	{
		System::String tipo = "void";
		advance();
		return tipo;
	}
	else
		return Type();
}
void Parser::ParametersOpt(struct FunctionSTRC *Var1){
	if (token->type == ')')
		return;

	Parameter(Var1);
	while (token->type == ',')
		Parameter(Var1);
}
void Parser::Parameter(struct FunctionSTRC *Var1){
	//Insere o parametro na lista
	Parser::StrcID Var2;
	Var2.Tipo		= Type();
	Var2.Escopo = this->Prefixo;
	Var2.ID			= matchIdentifier();
	Var2.Linha	= this->lineNumber;
	
	Var1->ListaParametros.push_back(Var2);
}