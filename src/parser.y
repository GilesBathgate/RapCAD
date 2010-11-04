/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010  Giles Bathgate
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see http://www.gnu.org/licenses/.
 */

%{
void parsererror(char const *);
int parserlex();
int lexerlex(void);
void lexererror(char const *);
int parse();
%}

%union {
        char *text;
}

%token MODULE FUNCTION
%token IF ELSE
%token <text> IDENTIFIER
%token <text> STRING
%token NUMBER
%token TRUE FALSE UNDEF
%token LE GE EQ NE AND OR

%right '?' ':'

%left OR
%left AND

%left '<' LE GE '>'
%left EQ NE

%left '!' '+' '-'
%left '*' '/' '%'
%left '[' ']'
%left '.'

%%
input
	: //empty
	|  declaration input
	;

declaration
	: single_statement
	| MODULE IDENTIFIER '(' parameters ')' module_body
	| FUNCTION IDENTIFIER '(' parameters ')' function_body
	;

module_body
	: compound_statement
	| module_instance
	;

function_body
	: '=' expression ';'
	| compound_statement
	;

statement
	: single_statement ';'
	| compound_statement
	;

single_statement
	: module_instance
	| assign_statement
	| ifelse_statement
	;

compound_statement
	: '{' '}'
	| '{' statement_list '}'
	;

statement_list
	: statement
	| statement_list statement
	;

assign_statement
	: IDENTIFIER '=' expression
	;

ifelse_statement
	: IF '(' expression ')' statement
	| IF '(' expression ')' statement ELSE statement
	;

for_statement
	: 
	;

expression
	: TRUE
	| FALSE
	| UNDEF
	| IDENTIFIER
	| expression '.' IDENTIFIER
	| STRING
	| NUMBER
	| '[' expression ':' expression ']'
	| expression '*' expression
	| expression '/' expression
	| expression '%' expression
	| expression '+' expression
	| expression '-' expression
	| expression '<' expression
	| expression LE expression
	| expression EQ expression
	| expression NE expression
	| expression GE expression
	| expression '>' expression
	| expression AND expression
	| expression OR expression
	;

parameters
	: //empty
	| parameter 
	| parameter ',' parameters
	;

parameter
	: IDENTIFIER
	| IDENTIFIER '=' expression
	;

module_instance
	: single_instance ';'
	| single_instance module_instance
	| single_instance '{' instance_list '}'
	;

instance_list
	: //empty
	| instance_list module_instance
	;

single_instance
	: IDENTIFIER '(' arguments ')'
	;

arguments
	: //empty
	| argument
	| argument ',' optional_commas arguments
	;

optional_commas
	: //empty
	| ',' optional_commas
	;

argument
	: IDENTIFIER
	| IDENTIFIER '=' expression
	;

%%

int parserlex(void)
{
    return lexerlex();
}

void parsererror(char const *)
{
}

int parse(const char *text)
{
    parserparse();

    return 0;
}
