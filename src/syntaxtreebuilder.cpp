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
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "syntaxtreebuilder.h"

SyntaxTreeBuilder::SyntaxTreeBuilder()
{
	script = new Script();
}

SyntaxTreeBuilder::~SyntaxTreeBuilder()
{
}

void SyntaxTreeBuilder::BuildScript(Declaration* dec)
{
	script->addDeclaration(dec);
}

Declaration* SyntaxTreeBuilder::BuildUse(QString* imp)
{
	ModuleImport* result = new ModuleImport();
	result->setImport(*imp);
	delete imp;
	return result;
}

Declaration* SyntaxTreeBuilder::BuildUse(QString* imp,QString* name)
{
	ModuleImport* result = new ModuleImport();
	result->setImport(*imp);
	delete imp;
	result->setNamespace(*name);
	delete name;

	return result;
}

void SyntaxTreeBuilder::BuildScript(QVector<Declaration*>* decls)
{
	script->setDeclarations(*decls);
	delete decls;
}

Declaration* SyntaxTreeBuilder::BuildDeclaration(Declaration* dec)
{
	return dec;
}

QVector<Declaration*>* SyntaxTreeBuilder::BuildDeclarations()
{
	return new QVector<Declaration*>();
}

QVector<Declaration*>* SyntaxTreeBuilder::BuildDeclarations(QVector<Declaration*>* decls,Declaration* decl)
{
	decls->append(decl);
	return decls;
}

QVector<Declaration*>* SyntaxTreeBuilder::BuildDeclarations(QVector<Declaration*>* decls)
{
	return decls;
}

QVector<Declaration*>* SyntaxTreeBuilder::BuildDeclarations(Declaration* decl)
{
	QVector<Declaration*>* result = new QVector<Declaration*>();
	result->append(decl);
	return result;
}

Statement* SyntaxTreeBuilder::BuildStatement(Statement* stmt)
{
	return stmt;
}

Statement* SyntaxTreeBuilder::BuildStatement(QVector<Statement*>* stmts)
{
	CompoundStatement* result = new CompoundStatement();
	result->setChildren(*stmts);
	delete stmts;
	return result;
}

Statement* SyntaxTreeBuilder::BuildStatement(Variable* var,Expression* exp)
{
	AssignStatement* result = new AssignStatement();
	result->setVariable(var);
	result->setExpression(exp);
	return result;
}

Statement* SyntaxTreeBuilder::BuildStatement(QString* name,Variable::Type_e type, Expression* exp)
{
	AssignStatement* result = new AssignStatement();
	Variable* var = new Variable();
	var->setName(*name);
	delete name;
	var->setType(type);
	result->setVariable(var);
	result->setExpression(exp);
	return result;
}

Statement* SyntaxTreeBuilder::BuildReturnStatement(Expression* exp)
{
	ReturnStatement* result = new ReturnStatement();
	result->setExpression(exp);
	return result;
}

Statement* SyntaxTreeBuilder::BuildIfElseStatement(Expression* expr,Statement* stmt)
{
	IfElseStatement* result = new IfElseStatement();
	result->setExpression(expr);
	result->setTrueStatement(stmt);
	return result;
}

Statement* SyntaxTreeBuilder::BuildIfElseStatement(Expression* expr,Statement* truestmt ,Statement* falsestmt)
{
	IfElseStatement* result = new IfElseStatement();
	result->setExpression(expr);
	result->setTrueStatement(truestmt);
	result->setFalseStatement(falsestmt);;
	return result;
}

Statement* SyntaxTreeBuilder::BuildForStatement(QVector<Argument*>* args,Statement* stmt)
{
	ForStatement* result = new ForStatement();
	result->setArguments(*args);
	delete args;
	result->setStatement(stmt);
	return result;
}

QVector<Statement*>* SyntaxTreeBuilder::BuildStatements()
{
	return new QVector<Statement*>();
}

QVector<Statement*>* SyntaxTreeBuilder::BuildStatements(Statement* stmt)
{
	QVector<Statement*>* result = new QVector<Statement*>();
	result->append(stmt);
	return result;
}

QVector<Statement*>* SyntaxTreeBuilder::BuildStatements(QVector<Statement*>* stmts)
{
	return stmts;
}

QVector<Statement*>* SyntaxTreeBuilder::BuildStatements(QVector<Statement*>* stmts,Statement* stmt)
{
	stmts->append(stmt);
	return stmts;
}

Declaration* SyntaxTreeBuilder::BuildModule(QString* name, QVector<Parameter*>* params, Scope* scp)
{
	Module* result = new Module();
	result->setName(*name);
	delete name;
	result->setParameters(*params);
	delete params;
	result->setScope(scp);
	return result;
}

Declaration* SyntaxTreeBuilder::BuildFunction(QString* name, QVector<Parameter*>* params, Scope* scp)
{
	Function* result = new Function();
	result->setName(*name);
	delete name;
	result->setParameters(*params);
	delete params;
	result->setScope(scp);
	return result;
}

Scope* SyntaxTreeBuilder::BuildScope(QVector<Declaration*>* decls)
{
	ModuleScope* result = new ModuleScope();
	result->setDeclarations(*decls);
	delete decls;
	return result;
}

Scope* SyntaxTreeBuilder::BuildScope(Instance* inst)
{
	ModuleScope* result = new ModuleScope();
	QVector<Declaration*> decls(1,inst);
	result->setDeclarations(decls);
	return result;
}

Scope* SyntaxTreeBuilder::BuildScope(Expression* expr)
{
	FunctionScope* result = new FunctionScope();
	result->setExpression(expr);
	return result;
}

Scope* SyntaxTreeBuilder::BuildScope(QVector<Statement*>* stmts)
{
	FunctionScope* result = new FunctionScope();
	result->setStatements(*stmts);
	delete stmts;
	return result;
}

Instance* SyntaxTreeBuilder::BuildInstance(Instance* inst)
{
	return inst;
}

Instance* SyntaxTreeBuilder::BuildInstance(QString* name,Instance* inst)
{
	inst->setNamespace(*name);
	delete name;
	return inst;
}

Instance* SyntaxTreeBuilder::BuildInstance(Instance::Type_e type,Instance* inst)
{
	inst->setType(type);
	return inst;
}

Instance* SyntaxTreeBuilder::BuildInstance(QString* name,QVector<Argument*>* args)
{
	Instance* result = new Instance();
	result->setName(*name);
	delete name;
	result->setArguments(*args);
	delete args;
	return result;
}

Instance* SyntaxTreeBuilder::BuildInstance(Instance* inst,QVector<Statement*>* smts)
{
	inst->setChildren(*smts);
	delete smts;
	return inst;
}

QVector<Parameter*>* SyntaxTreeBuilder::BuildParameters()
{
	return new QVector<Parameter*>();
}

QVector<Parameter*>* SyntaxTreeBuilder::BuildParameters(Parameter* param)
{
	QVector<Parameter*>* result = new QVector<Parameter*>();
	result->append(param);
	return result;
}

QVector<Parameter*>* SyntaxTreeBuilder::BuildParameters(QVector<Parameter*>* params,Parameter* param)
{
	params->append(param);
	return params;
}

Parameter* SyntaxTreeBuilder::BuildParameter(QString* name)
{
	Parameter* result = new Parameter();
	result->setName(*name);
	delete name;
	return result;
}

Parameter* SyntaxTreeBuilder::BuildParameter(QString* name,Expression* expr)
{
	Parameter* result = new Parameter();
	result->setName(*name);
	delete name;
	result->setExpression(expr);
	return result;
}

QVector<Argument*>* SyntaxTreeBuilder::BuildArguments()
{
	return new QVector<Argument*>();
}

QVector<Argument*>* SyntaxTreeBuilder::BuildArguments(Argument* arg)
{
	QVector<Argument*>* result = new QVector<Argument*>();
	result->append(arg);
	return result;
}

QVector<Argument*>* SyntaxTreeBuilder::BuildArguments(QVector<Argument*>* args,unsigned int count,Argument* arg)
{
	//if we are passed an empty list then we know that the first argument was blank.
	if(args->size()==0)
		args->append(new Argument());

	for(unsigned int i=0; i<count; i++)
		args->append(new Argument());

	args->append(arg);
	return args;
}

Argument* SyntaxTreeBuilder::BuildArgument(Expression* exp)
{
	Argument* result = new Argument();
	result->setExpression(exp);
	return result;
}

Argument* SyntaxTreeBuilder::BuildArgument(Variable* var,Expression* exp)
{
	Argument* result = new Argument();
	result->setVariable(var);
	result->setExpression(exp);
	return result;
}

unsigned int SyntaxTreeBuilder::BuildOptionalCommas()
{
	return 0;
}

unsigned int SyntaxTreeBuilder::BuildOptionalCommas(unsigned int count)
{
	return ++count;
}

Expression* SyntaxTreeBuilder::BuildLiteral()
{
	return new Literal();
}

Expression* SyntaxTreeBuilder::BuildLiteral(bool value)
{
	Literal* result = new Literal();
	result->setValue(value);
	return result;
}

Expression* SyntaxTreeBuilder::BuildLiteral(double value)
{
	Literal* result = new Literal();
	result->setValue(value);
	return result;
}

Expression* SyntaxTreeBuilder::BuildLiteral(QString* value)
{
	Literal* result = new Literal();
	result->setValue(*value);
	delete value;
	return result;
}

Variable* SyntaxTreeBuilder::BuildVariable(QString* name)
{
	Variable* result = new Variable();
	result->setName(*name);
	delete name;
	return result;
}

Expression* SyntaxTreeBuilder::BuildVariable(Variable* var)
{
	return var;
}

Variable* SyntaxTreeBuilder::BuildVariable(QString* name,Variable::Type_e type)
{
	Variable* result = new Variable();
	result->setType(type);
	result->setName(*name);
	delete name;
	return result;
}

Expression* SyntaxTreeBuilder::BuildExpression(Expression* exp,QString* name)
{
	BinaryExpression* result = new BinaryExpression();
	result->setLeft(exp);
	Variable* val = new Variable();
	val->setName(*name);
	delete name;
	result->setRight(val);
	result->setOp(Expression::Dot);
	return result;
}

Expression* SyntaxTreeBuilder::BuildExpression(Expression* exp)
{
	return exp;
}

Expression* SyntaxTreeBuilder::BuildExpression(Expression::Operator_e op,Expression* exp)
{
	UnaryExpression* result = new UnaryExpression();
	result->setExpression(exp);
	result->setOp(op);
	return result;
}

Expression* SyntaxTreeBuilder::BuildExpression(Expression* left ,Expression::Operator_e op, Expression* right)
{
	BinaryExpression* result = new BinaryExpression();
	result->setLeft(left);
	result->setRight(right);
	result->setOp(op);
	return result;
}

Expression* SyntaxTreeBuilder::BuildTernaryExpression(Expression* cond,Expression* trueexp ,Expression* falseexp)
{
	TernaryExpression* result = new TernaryExpression();
	result->setCondition(cond);
	result->setTrueExpression(trueexp);
	result->setFalseExpression(falseexp);
	return result;
}

Expression* SyntaxTreeBuilder::BuildExpression(QVector<Expression*>* exps)
{
	VectorExpression* result = new VectorExpression();
	result->setChildren(*exps);
	delete exps;
	return result;
}

QVector<Expression*>* SyntaxTreeBuilder::BuildVector()
{
	return new QVector<Expression*>();
}

QVector<Expression*>* SyntaxTreeBuilder::BuildVector(Expression* exp)
{
	QVector<Expression*>* result = new QVector<Expression*>();
	result->append(exp);
	return result;
}

QVector<Expression*>* SyntaxTreeBuilder::BuildVector(QVector<Expression*>* exprs,unsigned int count,Expression* expr)
{
	if(exprs->size()==0)
		exprs->append(new Variable());

	for(unsigned int i=0; i<count; i++)
		exprs->append(new Variable());
	exprs->append(expr);
	return exprs;
}

Expression* SyntaxTreeBuilder::BuildRange(Expression* srt,Expression* fin)
{
	RangeExpression* result = new RangeExpression();
	result->setStart(srt);
	result->setFinish(fin);
	return result;
}

Expression* SyntaxTreeBuilder::BuildRange(Expression* srt,Expression* stp,Expression* fin)
{
	RangeExpression* result = new RangeExpression();
	result->setStart(srt);
	result->setFinish(fin);
	result->setStep(stp);
	return result;
}

Expression* SyntaxTreeBuilder::BuildInvocation(QString* name,QVector<Argument*>* args)
{
	Invocation* result = new Invocation();
	result->setName(*name);
	delete name;
	result->setArguments(*args);
	delete args;
	return result;
}

Script* SyntaxTreeBuilder::getResult()
{
	return script;
}
