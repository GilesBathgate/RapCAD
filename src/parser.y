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

%expect 1 // Dangling else problem causes 1 shift/reduce conflict

%{
#include <stdio.h>
#include <QString>
#include <QVector>
#include <QDir>

#include "abstractsyntaxtreebuilder.h"
#include "syntaxtreebuilder.h"
#include "dependencybuilder.h"
#include "script.h"
#include "prettyprinter.h"

extern FILE* lexerin;
extern int lexerlineno;
extern int lexerlex(void);
extern char *lexertext;
struct yy_buffer_state;
extern yy_buffer_state* lexer_scan_string(const char *);
extern void lexer_delete_buffer(yy_buffer_state*);
extern void lexerinit(QDir);

void parsererror(char const *);
int parserlex();
void parse(QString,bool);

AbstractSyntaxTreeBuilder *builder;
%}

%union {
	QString* text;
	double number;
	unsigned int count;
	class Declaration* decl;
	class QVector<Declaration*>* decls;
	class Parameter* param;
	class QVector<Parameter*>* params;
	class Argument* arg;
	class QVector<Argument*>* args;
	class Statement* stmt;
	class QVector<Statement*>* stmts;
	class Expression* expr;
	class QVector<Expression*>* exprs;
	class Instance* inst;
	class Scope* scp;
	class Value* var;
}
%token <text> USE
%token MODULE FUNCTION
%token IF ELSE
%token FOR
%token CONST PARAM
%token <text> IDENTIFIER
%token <text> STRING
%token <number> NUMBER
%token TOK_TRUE TOK_FALSE UNDEF

%right RETURN
%right '='
%right '?' ':'

%left OR
%left AND

%left '<' LE GE '>'
%left EQ NE

%left '!' '+' '-'
%left '*' '/' '%'
%left '[' ']'
%left '.'

%type <count> optional_commas
%type <decl>  declaration
%type <decls>  declaration_list compound_declaration
%type <param> parameter
%type <params> parameters
%type <arg> argument
%type <args> arguments
%type <expr> expression
%type <exprs> vector_expression
%type <scp> module_scope function_scope
%type <inst> module_instance single_instance
%type <var> variable
%type <stmt> statement single_statement assign_statement return_statement ifelse_statement for_statement
%type <stmts> compound_statement statement_list compound_instance

%%
input
	: //empty
	| USE input
	{ builder->BuildScript($1); }
	| declaration_list
	{ builder->BuildScript($1); }
	;

declaration_list
	: declaration
	{ $$ = builder->BuildDeclarations($1); }
	| declaration_list declaration
	{ $$ = builder->BuildDeclarations($1,$2); }
	;

compound_declaration
	: '{' '}'
	{ $$ = builder->BuildDeclarations(); }
	| '{' declaration_list '}'
	{ $$ = builder->BuildDeclarations($2); }
	;

declaration
	: single_statement
	{ $$ = builder->BuildStatement($1); }
	| MODULE IDENTIFIER '(' parameters ')' module_scope
	{ $$ = builder->BuildModule($2,$4,$6); }
	| FUNCTION IDENTIFIER '(' parameters ')' function_scope
	{ $$ = builder->BuildFunction($2,$4,$6); }
	;

module_scope
	: compound_declaration
	{ $$ = builder->BuildScope($1); }
	| module_instance
	{ $$ = builder->BuildScope($1); }
	;

function_scope
	: '=' expression ';'
	{ $$ = builder->BuildScope($2); }
	| compound_statement
	{ $$ = builder->BuildScope($1); }
	;

statement
	: single_statement
	{ $$ = builder->BuildStatement($1); }
	| compound_statement
	{ $$ = builder->BuildStatement($1); }
	;

single_statement
	: module_instance
	{ $$ = builder->BuildInstance($1); }
	| assign_statement ';'
	{ $$ = builder->BuildStatement($1); }
	| ifelse_statement
	{ $$ = builder->BuildStatement($1); }
	| for_statement
	{ $$ = builder->BuildStatement($1); }
        | return_statement;
        ;

return_statement
        : RETURN expression ';'
        { $$ = builder->BuildReturnStatement($2); }
	;

compound_statement
	: '{' '}'
	{ $$ = builder->BuildStatements(); }
	| '{' statement_list '}'
	{ $$ = builder->BuildStatements($2); }
	;

statement_list
	: statement
	{ $$ = builder->BuildStatements($1); }
	| statement_list statement
	{ $$ = builder->BuildStatements($1,$2); }
	;

assign_statement
	: variable '=' expression
	{ $$ = builder->BuildStatement($1,$3); }
	| CONST IDENTIFIER '=' expression
	{ $$ = builder->BuildStatement($2,Value::Const,$4); }
	| PARAM IDENTIFIER '=' expression
	{ $$ = builder->BuildStatement($2,Value::Param,$4); }
	;

ifelse_statement
	: IF '(' expression ')' statement
	{ $$ = builder->BuildIfElseStatement($3,$5); }
	| IF '(' expression ')' statement ELSE statement
	{ $$ = builder->BuildIfElseStatement($3,$5,$7); }
	;

for_statement
	: FOR '(' arguments ')' statement
	{ $$ = builder->BuildForStatement($3,$5); }
	;

variable
	: IDENTIFIER
	{ $$ = builder->BuildVariable($1); }
	| '$' IDENTIFIER
	{ $$ = builder->BuildVariable($2,Value::Special); }
	;

expression
	: TOK_TRUE
	{ $$ = builder->BuildLiteral(true); }
	| TOK_FALSE
	{ $$ = builder->BuildLiteral(false); }
	| UNDEF
	{ $$ = builder->BuildLiteral(); }
	| variable
	{ $$ = builder->BuildVariable($1); }
	| expression '.' IDENTIFIER
	{ $$ = builder->BuildExpression($1,$3); }
	| STRING
	{ $$ = builder->BuildLiteral($1); }
	| NUMBER
	{ $$ = builder->BuildLiteral($1); }
	| '[' expression ':' expression ']'
	{ $$ = builder->BuildRange($2,$4); }
	| '[' expression ':' expression ':' expression ']'
	{ $$ = builder->BuildRange($2,$4,$6); }
	| '[' vector_expression ']'
	{ $$ = builder->BuildExpression($2); }
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
	| '+' expression
        { $$ = builder->BuildExpression(Expression::Add,$2); }
	| '-' expression
        { $$ = builder->BuildExpression(Expression::Subtract,$2); }
	| '!' expression
        { $$ = builder->BuildExpression(Expression::Invert,$2); }
	| '(' expression ')'
        { $$ = builder->BuildExpression($2); }
	| expression '?' expression ':' expression
        { $$ = builder->BuildTernaryExpression($1,$3,$5); }
	| expression '[' expression ']'
        { $$ = builder->BuildExpression($1,Expression::Index,$3); }
	| IDENTIFIER '(' arguments ')'
        { $$ = builder->BuildInvocation($1,$3); }
	;

vector_expression
	: //empty
	{ $$ = builder->BuildVector(); }
	| expression
	{ $$ = builder->BuildVector($1); }
	| vector_expression ',' optional_commas expression
	{ $$ = builder->BuildVector($1,$3,$4); }
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
	: '{' '}'
	{ $$ = builder->BuildStatements(); }
	| '{' statement_list '}'
	{ $$ = builder->BuildStatements($2); }
	| module_instance
	{ $$ = builder->BuildStatements($1); }
	;

module_instance
	: single_instance ';'
	{ $$ = builder->BuildInstance($1); }
	| single_instance compound_instance
	{ $$ = builder->BuildInstance($1,$2); }
	;

single_instance
	: IDENTIFIER '(' arguments ')'
	{ $$ = builder->BuildInstance($1,$3); }
	| '!' single_instance
	{ $$ = builder->BuildInstance(Instance::Root,$2); }
	| '#' single_instance
	{ $$ = builder->BuildInstance(Instance::Debug,$2); }
	| '%' single_instance
	{ $$ = builder->BuildInstance(Instance::Background,$2); }
	| '*' single_instance
	{ $$ = builder->BuildInstance(Instance::Disable,$2); }
	;

arguments
	: //empty
	{ $$ = builder->BuildArguments(); }
	| argument
	{ $$ = builder->BuildArguments($1); }
	| arguments ',' optional_commas argument
	{ $$ = builder->BuildArguments($1,$3,$4); }
	;

optional_commas
	: //empty
	{ $$ = builder->BuildOptionalCommas(); }
	| ',' optional_commas
	{ $$ = builder->BuildOptionalCommas($2); }
	;

argument
	: expression
	{ $$ = builder->BuildArgument($1); }
	| variable '=' expression
	{ $$ = builder->BuildArgument($1,$3); }
	;

%%

int parserlex(void)
{
	return lexerlex();
}

void parsererror(char const *s)
{
	fprintf(stderr,"%d: %s at %s\n", lexerlineno, s, lexertext);
}

void parse(QString input,bool file)
{
	builder = new SyntaxTreeBuilder();

	if(file)
	{
	    QFileInfo fileinfo(input);
	    lexerinit(fileinfo.absoluteDir());
	    const char* fullpath = fileinfo.absoluteFilePath().toLocal8Bit();
	    lexerin = fopen(fullpath,"r");
	    parserparse();
	    fclose(lexerin);
	}
	else
	{
	    QDir current;
	    lexerinit(current);
	    yy_buffer_state* str_buffer = lexer_scan_string(input.toLocal8Bit());
	    parserparse();
	    lexer_delete_buffer(str_buffer);
	}

	Script* s = builder->getResult();
	delete builder;

	PrettyPrinter p(s);
	p.Print();

	//TODO just delete the script for now
	delete s;
}
