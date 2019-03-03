/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2019 Giles Bathgate
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
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

%{
#include <QString>
#include <QList>
#include "decimal.h"
#include "syntaxtreebuilder.h"
#include "tokenbuilder.h"
#include "script.h"
#include "reporter.h"

void parsescript(Script&,Reporter&,QFileInfo);
void parsescript(Script&,Reporter&,const QString&);

static void parsererror(const char*);
static int parserlex();
static AbstractSyntaxTreeBuilder* builder;
static AbstractTokenBuilder* tokenizer;

%}

%union {
	QString* text;
	decimal* number;
	unsigned int count;
	class Declaration* decl;
	class QList<Declaration*>* decls;
	class Parameter* param;
	class QList<Parameter*>* params;
	class Argument* arg;
	class QList<Argument*>* args;
	class Statement* stmt;
	class QList<Statement*>* stmts;
	class Expression* expr;
	class QList<Expression*>* exprs;
	class Instance* inst;
	class Scope* scp;
	class Variable* var;
	class Invocation* inv;
	class QList<class CodeDoc*>* cdocs;
}
%token DOCSTART DOCEND
%token <text> DOCPARAM
%token <text> DOCTEXT
%token <text> USE
%token <text> IMPORT
%token MODULE FUNCTION
%token TOK_IF
%right THEN ELSE
%token FOR
%token CONST PARAM
%token <text> IDENTIFIER
%token <text> STRING
%token <number> NUMBER
%token TOK_TRUE TOK_FALSE UNDEF
%token TOK_AS NS

%right RETURN
%right '=' APPEND
%right '?' ':'
%left OR
%left AND
%left '<' LE GE '>'
%left EQ NE
%left '!' '+' '-' '~' '|'
%left '*' '/' '%'
%left INC DEC ADDA SUBA
%left CM CD CP
%right '^'
%left '[' ']'
%left '.'

%type <cdocs> codedoc codedoc_param
%type <count> optional_commas
%type <decl> declaration use_declaration import_declaration single_declaration define_declaration
%type <decls> declaration_list single_declaration_list compound_declaration
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
%type <inv> invocation

%%
input
	: //empty
	| use_declaration input
	{ builder->buildScript($1); }
	| import_declaration input
	{ builder->buildScript($1); }
	| single_declaration_list
	{ builder->buildScript($1); }
	| codedoc input
	{ builder->buildScript($1); }
	;

codedoc
	: DOCSTART codedoc_param DOCEND
	{ $$ = builder->buildCodeDoc($2); }
	;

codedoc_param
	: //empty
	{ $$ = builder->buildCodeDoc(); }
	| DOCTEXT codedoc_param
	{ $$ = builder->buildCodeDoc($1,$2); }
	| DOCPARAM DOCTEXT codedoc_param
	{ $$ = builder->buildCodeDoc($1,$2,$3); }
	;

use_declaration
	: USE
	{ $$ = builder->buildUse($1); }
	| USE TOK_AS IDENTIFIER ';'
	{ $$ = builder->buildUse($1,$3); }
	;

import_declaration
	: IMPORT TOK_AS IDENTIFIER ';'
	{ $$ = builder->buildImport($1,$3); }
	| IMPORT TOK_AS IDENTIFIER '(' parameters ')' ';'
	{ $$ = builder->buildImport($1,$3,$5); }
	;


single_declaration_list
	: single_declaration
	{ $$ = builder->buildDeclarations($1); }
	| single_declaration_list single_declaration
	{ $$ = builder->buildDeclarations($1,$2); }
	;

declaration_list
	: declaration
	{ $$ = builder->buildDeclarations($1); }
	| declaration_list declaration
	{ $$ = builder->buildDeclarations($1,$2); }
	;

compound_declaration
	: '{' '}'
	{ $$ = builder->buildDeclarations(); }
	| '{' declaration_list '}'
	{ $$ = builder->buildDeclarations($2); }
	;

single_declaration
	: single_statement
	{ $$ = builder->buildStatement($1); }
	| define_declaration
	{ $$ = builder->buildDeclaration($1); }
	;

declaration
	: statement
	{ $$ = builder->buildStatement($1); }
	| define_declaration
	{ $$ = builder->buildDeclaration($1); }
	;

define_declaration
	: MODULE IDENTIFIER '(' parameters ')' module_scope
	{ $$ = builder->buildModule($2,$4,$6); }
	| FUNCTION IDENTIFIER '(' parameters ')' function_scope
	{ $$ = builder->buildFunction($2,$4,$6); }
	;

module_scope
	: compound_declaration
	{ $$ = builder->buildScope($1); }
	| module_instance
	{ $$ = builder->buildScope($1); }
	;

function_scope
	: '=' expression ';'
	{ $$ = builder->buildScope($2); }
	| compound_statement
	{ $$ = builder->buildScope($1); }
	;

statement
	: single_statement
	{ $$ = builder->buildStatement($1); }
	| compound_statement
	{ $$ = builder->buildStatement($1); }
	;

single_statement
	: module_instance
	{ $$ = builder->buildInstance($1); }
	| assign_statement ';'
	{ $$ = builder->buildStatement($1); }
	| ifelse_statement
	{ $$ = builder->buildStatement($1); }
	| for_statement
	{ $$ = builder->buildStatement($1); }
	| return_statement
	{ $$ = builder->buildStatement($1); }
	;

return_statement
	: RETURN expression ';'
	{ $$ = builder->buildReturnStatement($2); }
	;

compound_statement
	: '{' '}'
	{ $$ = builder->buildStatements(); }
	| '{' statement_list '}'
	{ $$ = builder->buildStatements($2); }
	;

statement_list
	: statement
	{ $$ = builder->buildStatements($1); }
	| statement_list statement
	{ $$ = builder->buildStatements($1,$2); }
	;

assign_statement
	: variable '=' expression
	{ $$ = builder->buildStatement($1,$3); }
	| variable APPEND expression
	{ $$ = builder->buildStatement($1,Expression::Append,$3); }
	| variable INC
	{ $$ = builder->buildStatement($1,Expression::Increment); }
	| variable DEC
	{ $$ = builder->buildStatement($1,Expression::Decrement); }
	| variable ADDA expression
	{ $$ = builder->buildStatement($1,Expression::AddAssign,$3); }
	| variable SUBA expression
	{ $$ = builder->buildStatement($1,Expression::SubAssign,$3); }
	| CONST IDENTIFIER '=' expression
	{ $$ = builder->buildStatement($2,Variable::Const,$4); }
	| PARAM IDENTIFIER '=' expression
	{ $$ = builder->buildStatement($2,Variable::Param,$4); }
	;

ifelse_statement
	: TOK_IF '(' expression ')' statement %prec THEN
	{ $$ = builder->buildIfElseStatement($3,$5); }
	| TOK_IF '(' expression ')' statement ELSE statement
	{ $$ = builder->buildIfElseStatement($3,$5,$7); }
	;

for_statement
	: FOR '(' arguments ')' statement
	{ $$ = builder->buildForStatement($3,$5); }
	;

variable
	: IDENTIFIER
	{ $$ = builder->buildVariable($1); }
	| '$' IDENTIFIER
	{ $$ = builder->buildVariable($2,Variable::Special); }
	;

expression
	: TOK_TRUE
	{ $$ = builder->buildLiteral(true); }
	| TOK_FALSE
	{ $$ = builder->buildLiteral(false); }
	| UNDEF
	{ $$ = builder->buildLiteral(); }
	| variable
	{ $$ = builder->buildVariable($1); }
	| expression '.' IDENTIFIER
	{ $$ = builder->buildExpression($1,$3); }
	| STRING
	{ $$ = builder->buildLiteral($1); }
	| NUMBER
	{ $$ = builder->buildLiteral($1); }
	| NUMBER IDENTIFIER
	{ $$ = builder->buildLiteral($1,$2); }
	| '[' expression ':' expression ']'
	{ $$ = builder->buildRange($2,$4); }
	| '[' expression ':' expression ':' expression ']'
	{ $$ = builder->buildRange($2,$4,$6); }
	| '[' vector_expression optional_commas ']'
	{ $$ = builder->buildExpression($2,$3); }
	| '<' expression ',' expression ',' expression ',' expression '>'
	{ $$ = builder->buildComplex($2,$4,$6,$8); }
	| '|' expression '|'
	{ $$ = builder->buildExpression(Expression::Length,$2); }
	| expression '^' expression
	{ $$ = builder->buildExpression($1,Expression::Exponent,$3); }
	| expression '*' expression
	{ $$ = builder->buildExpression($1,Expression::Multiply,$3); }
	| expression '~' expression
	{ $$ = builder->buildExpression($1,Expression::Concatenate,$3); }
	| expression CM expression
	{ $$ = builder->buildExpression($1,Expression::ComponentwiseMultiply,$3); }
	| expression '/' expression
	{ $$ = builder->buildExpression($1,Expression::Divide,$3); }
	| expression CD expression
	{ $$ = builder->buildExpression($1,Expression::ComponentwiseDivide,$3); }
	| expression CP expression
	{ $$ = builder->buildExpression($1,Expression::CrossProduct,$3); }
	| expression '%' expression
	{ $$ = builder->buildExpression($1,Expression::Modulus,$3); }
	| expression '+' expression
	{ $$ = builder->buildExpression($1,Expression::Add,$3); }
	| expression '-' expression
	{ $$ = builder->buildExpression($1,Expression::Subtract,$3); }
	| expression '<' expression
	{ $$ = builder->buildExpression($1,Expression::LessThan,$3); }
	| expression LE expression
	{ $$ = builder->buildExpression($1,Expression::LessOrEqual,$3); }
	| expression EQ expression
	{ $$ = builder->buildExpression($1,Expression::Equal,$3); }
	| expression NE expression
	{ $$ = builder->buildExpression($1,Expression::NotEqual,$3); }
	| expression GE expression
	{ $$ = builder->buildExpression($1,Expression::GreaterOrEqual,$3); }
	| expression '>' expression
	{ $$ = builder->buildExpression($1,Expression::GreaterThan,$3); }
	| expression AND expression
	{ $$ = builder->buildExpression($1,Expression::LogicalAnd,$3); }
	| expression OR expression
	{ $$ = builder->buildExpression($1,Expression::LogicalOr,$3); }
	| expression INC
	{ $$ = builder->buildExpression(Expression::Increment,$1); }
	| expression DEC
	{ $$ = builder->buildExpression(Expression::Decrement,$1); }
	| '+' expression
	{ $$ = builder->buildExpression(Expression::Add,$2); }
	| '-' expression
	{ $$ = builder->buildExpression(Expression::Subtract,$2); }
	| '!' expression
	{ $$ = builder->buildExpression(Expression::Invert,$2); }
	| '(' expression ')'
	{ $$ = builder->buildExpression($2); }
	| expression '?' expression ':' expression
	{ $$ = builder->buildExpression($1,$3,$5); }
	| expression '[' expression ']'
	{ $$ = builder->buildExpression($1,Expression::Index,$3); }
	| invocation
	{ $$ = builder->buildExpression($1); }
	;

invocation
	: IDENTIFIER NS invocation
	{ $$ = builder->buildInvocation($1,$3); }
	| IDENTIFIER '(' arguments ')'
	{ $$ = builder->buildInvocation($1,$3); }
	;

vector_expression
	: //empty
	{ $$ = builder->buildVector(); }
	| expression
	{ $$ = builder->buildVector($1); }
	| vector_expression ',' optional_commas expression
	{ $$ = builder->buildVector($1,$3,$4); }
	;

parameters
	: //empty
	{ $$ = builder->buildParameters(); }
	| parameter
	{ $$ = builder->buildParameters($1); }
	| parameters ',' parameter
	{ $$ = builder->buildParameters($1,$3); }
	;

parameter
	: IDENTIFIER
	{ $$ = builder->buildParameter($1); }
	| IDENTIFIER '=' expression
	{ $$ = builder->buildParameter($1,$3); }
	;

compound_instance
	: compound_statement
	{ $$ = builder->buildStatements($1); }
	| ifelse_statement
	{ $$ = builder->buildStatements($1); }
	| for_statement
	{ $$ = builder->buildStatements($1); }
	| module_instance
	{ $$ = builder->buildStatements($1); }
	;

module_instance
	: single_instance ';'
	{ $$ = builder->buildInstance($1); }
	| single_instance compound_instance
	{ $$ = builder->buildInstance($1,$2); }
	;

single_instance
	: IDENTIFIER NS single_instance
	{ $$ = builder->buildInstance($1,$3); }
	| IDENTIFIER '(' arguments ')'
	{ $$ = builder->buildInstance($1,$3); }
	| IDENTIFIER '$' '(' arguments ')'
	{ $$ = builder->buildInstance(Instance::Auxilary,$1,$4); }
	| '!' single_instance
	{ $$ = builder->buildInstance(Instance::Root,$2); }
	| '#' single_instance
	{ $$ = builder->buildInstance(Instance::Debug,$2); }
	| '%' single_instance
	{ $$ = builder->buildInstance(Instance::Background,$2); }
	| '*' single_instance
	{ $$ = builder->buildInstance(Instance::Disable,$2); }
	;

arguments
	: //empty
	{ $$ = builder->buildArguments(); }
	| argument
	{ $$ = builder->buildArguments($1); }
	| arguments ',' optional_commas argument
	{ $$ = builder->buildArguments($1,$3,$4); }
	;

optional_commas
	: //empty
	{ $$ = builder->buildOptionalCommas(); }
	| ',' optional_commas
	{ $$ = builder->buildOptionalCommas($2); }
	;

argument
	: expression
	{ $$ = builder->buildArgument($1); }
	| variable '=' expression
	{ $$ = builder->buildArgument($1,$3); }
	;

%%

static int parserlex()
{
	return tokenizer->nextToken();
}

static void parsererror(const char* s)
{
	if(builder)
		builder->reportSyntaxError(s);
}

void parsescript(Script& s,Reporter& r,QFileInfo input)
{
	tokenizer=new TokenBuilder(r,input);
	builder=new SyntaxTreeBuilder(r,s,*tokenizer);
	builder->buildFileLocation(input.absoluteDir());

	parserparse();

	delete builder;
	delete tokenizer;
}

void parsescript(Script& s,Reporter& r,const QString& input)
{
	tokenizer=new TokenBuilder(r,input);
	builder=new SyntaxTreeBuilder(r,s,*tokenizer);

	parserparse();

	delete builder;
	delete tokenizer;
}
