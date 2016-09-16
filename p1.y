//Victor Gaíva

start:
  grammar_especifications_opt
  ;

grammar_especifications_opt:
  grammar_especifications*

grammar_especifications:
  bnf_productions
| TOKEN '{' token_definitions '}'

bnf_productions:
  VOID IDENTIFIER '(' ')' ':' '{' alternatives '}'

alternatives:
  alternative ('|' alternative)*

alternative:
  local_lookahead? expansion?

local_lookahead:
  LOOKAHEAD '(' INTEGER ')'
| LOOKAHEAD '(' alternatives ')'
    
expansion:
  expansion_unit expansion
    
expansion_unit:
  token
| IDENTIFIER '(' ')'
| '(' alternatives ')' ER_OP
| '(' alternatives ')'
    
token:
  STRING
| '<' IDENTIFIER '>'
| '<' EOF '>'
| '<' regular_expression '>'

token_definitions:
  token_definition ('|'token_definition)*

token_definition:
  '<' token_name ':' regular_expression '>'

token_name:
  IDENTIFIER
| '#' IDENTIFIER

regular_expression:
  regular_term ('|' regular_term)*

regular_term:
  (regular_factor)+

regular_factor:
  STRING
| '<' IDENTIFIER '>'
| char_set
| '(' regular_expression ')' (ER_OP)?

char_set:
 ('~')? '[' char_range_list ']'
    
char_range_list:
  char_range (',' char_range)*
    
char_range:
  STRING ('-' STRING)*
  ;
