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
#include <stdio.h>
#include <QVector>
#include "syntaxtreebuilder.h"
#include "parameter.h"
#include "expression.h"

void parsererror(char const *);
int parserlex();
int lexerlex(void);
int parse();
extern FILE* lexerin;

SyntaxTreeBuilder *builder;
%}

%union {
        char *text;
	double number;
	class Expression* expr;
	class QVector<Parameter*>* params;
	class Parameter* param;
}

%token MODULE FUNCTION
%token IF ELSE
%token FOR
%token CONST PARAM
%token <text> IDENTIFIER
%token <text> STRING
%token <number> NUMBER
%token TOK_TRUE TOK_FALSE UNDEF
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

%type <params> parameters
%type <param> parameter
%type <expr>  expression

%%
input
	: //empty
	|  input declaration
	;

declaration
	: single_statement
	| MODULE IDENTIFIER '(' parameters ')' module_body
	{ builder->BuildModule($2,$4); }
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
	| for_statement
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
	: CONST IDENTIFIER '=' expression
	| PARAM IDENTIFIER '=' expression
	| IDENTIFIER '=' expression
	;

ifelse_statement
	: IF '(' expression ')' statement
	| IF '(' expression ')' statement ELSE statement
	;

for_statement
	: FOR '(' expression ')' statement
	;

expression
	: TOK_TRUE
	{ $$ = builder->BuildLiteral(true); }
	| TOK_FALSE
	{ $$ = builder->BuildLiteral(false); }
	| UNDEF
	{ $$ = builder->BuildLiteral(); }
	| IDENTIFIER
	{ $$ = builder->BuildVariable($1); }
	| expression '.' IDENTIFIER
	| STRING
	{ $$ = builder->BuildLiteral($1); }
	| NUMBER
	{ $$ = builder->BuildLiteral($1); }
	| '[' expression ':' expression ']'
	| expression '*' expression
	{ $$ = builder->BuildExpression($1,Expression::Multiply,$3); }
	| expression '/' expression
	{ $$ = builder->BuildExpression($1,Expression::Divide,$3); }
	| expression '%' expression
	{ $$ = builder->BuildExpression($1,Expression::Modulus,$3); }
	| expression '+' expression
	{ $$ = builder->BuildExpression($1,Expression::Add,$3); }
	| expression '-' expression
	{ $$ = builder->BuildExpression($1,Expression::Subtract,$3); }
	| expression '<' expression
	{ $$ = builder->BuildExpression($1,Expression::LessThan,$3); }
	| expression LE expression
	{ $$ = builder->BuildExpression($1,Expression::LessOrEqual,$3); }
	| expression EQ expression
	{ $$ = builder->BuildExpression($1,Expression::Equal,$3); }
	| expression NE expression
	{ $$ = builder->BuildExpression($1,Expression::NotEqual,$3); }
	| expression GE expression
	{ $$ = builder->BuildExpression($1,Expression::GreaterOrEqual,$3); }
	| expression '>' expression
	{ $$ = builder->BuildExpression($1,Expression::GreaterThan,$3); }
	| expression AND expression
	{ $$ = builder->BuildExpression($1,Expression::LogicalAnd,$3); }
	| expression OR expression
	{ $$ = builder->BuildExpression($1,Expression::LogicalOr,$3); }
	;

parameters
	: //empty
	{ $$ = builder->BuildParameters(); }
	| parameter 
	{ $$ = builder->BuildParameters($1); }
	| parameters ',' parameter
	{ $$ = builder->BuildParameters($1,$3); }
	;

parameter
	: IDENTIFIER
	{ $$ = builder->BuildParameter($1); }
	| IDENTIFIER '=' expression
	{ $$ = builder->BuildParameter($1,$3); }
	;

compound_instance
	: module_instance
	| '{' instance_list '}'
	;

module_instance
	: single_instance ';'
	| single_instance compound_instance
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
	| arguments ',' optional_commas argument
	;

optional_commas
	: //empty
	| ',' optional_commas
	;

argument
	: expression
	| IDENTIFIER '=' expression
	;

%%

int parserlex(void)
{
	return lexerlex();
}

void parsererror(char const *s)
{
	fprintf (stderr, "%s\n", s);
}

int parse(const char *file)
{
	builder = new SyntaxTreeBuilder();
	lexerin = fopen(file,"r");
 	parserparse();
	printf("\nDone.\n");
	return 0;
}
