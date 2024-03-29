/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2023 Giles Bathgate
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

#include "treeprinter.h"
#include "onceonly.h"

TreePrinter::TreePrinter(QTextStream& s) :
	result(s),
	indent(0)
{
}

void TreePrinter::createIndent()
{
	for(unsigned int i=0; i<indent; ++i)
		result << "  ";
}

void TreePrinter::visit(const ModuleScope& scp)
{
	++indent;
	for(Declaration* d: scp.getDeclarations()) {
		createIndent();
		d->accept(*this);
	}
	--indent;
}

void TreePrinter::visit(const Instance& inst)
{

	switch(inst.getType()) {
		case InstanceTypes::Root:
			result << "!";
			break;
		case InstanceTypes::Debug:
			result << "#";
			break;
		case InstanceTypes::Background:
			result << "%";
			break;
		case InstanceTypes::Disable:
			result << "*";
			break;
		default:
			break;
	}

	const QString& name = inst.getNamespace();
	if(!name.isEmpty()) {
		result << name;
		result << "::";
	}
	result << inst.getName();
	result << "(";
	const QList<Argument*>& arguments = inst.getArguments();
	OnceOnly first;
	for(Argument* a: arguments) {
		if(!first())
			result << ",";
		a->accept(*this);
	}
	result << ")";

	const QList<Statement*>& children = inst.getChildren();
	const auto c=children.size();
	if(c>0) {
		if(c>1) {
			result << "{\n";
			++indent;
		}
		for(auto i=0; i<c; ++i) {
			if(c>1)
				createIndent();
			children.at(i)->accept(*this);
		}
		if(c>1) {
			--indent;
			createIndent();
			result << "}";
		}
	} else {
		result << ";";
	}

	result << "\n";
}

void TreePrinter::printParameters(const QList<Parameter*>& parameters)
{
	OnceOnly first;
	for(Parameter* p: parameters) {
		if(!first())
			result << ", ";
		p->accept(*this);
	}
}

void TreePrinter::visit(const Module& mod)
{
	if(mod.isDeprecated()) return;

	const QList<Parameter*>& parameters = mod.getParameters();
	const QString& desc=mod.getDescription();
	printCodeDoc(desc,parameters);
	result << "module ";
	result << mod.getFullName();
	result << "(";
	printParameters(parameters);
	result << "){";
	Scope* scp=mod.getScope();
	if(scp) {
		result << "\n";
		scp->accept(*this);
		createIndent();
	}
	result << "}\n\n";
}

void TreePrinter::visit(const Function& func)
{
	const QList<Parameter*>& parameters = func.getParameters();
	const QString& desc=func.getDescription();
	printCodeDoc(desc,parameters);
	result << "function ";
	result << func.getName();
	result << "(";
	printParameters(parameters);
	result << ")";
	Scope* scp=func.getScope();
	if(scp)
		scp->accept(*this);
	else
		result << "{}";
	result << "\n\n";
}

void TreePrinter::visit(const FunctionScope& scp)
{
	Expression* expression = scp.getExpression();
	if(expression) {
		result << "=";
		expression->accept(*this);
		result << ";\n";
		return;
	}

	const QList<Statement*>& statements = scp.getStatements();
	const auto size=statements.size();
	if(size>0) {
		result << "{\n";
		++indent;
		for(Statement* s: statements) {
			createIndent();
			s->accept(*this);
		}
		--indent;
		createIndent();
		result << "}";
	} else {
		result << ";";
	}
	result << "\n";
}

void TreePrinter::visit(const CompoundStatement& stmt)
{
	const QList<Statement*>& children = stmt.getChildren();
	const auto c=children.size();
	if(c>0) {
		if(c>1) {
			result << "{\n";
			++indent;
		}
		for(auto i=0; i<c; ++i) {
			if(c>1)
				createIndent();
			children.at(i)->accept(*this);
		}
		if(c>1) {
			--indent;
			createIndent();
			result << "}";
		}
	} else
		result << ";";

}

void TreePrinter::visit(const IfElseStatement& ifelse)
{
	result << "if(";
	ifelse.getExpression()->accept(*this);
	result << ")";
	Statement* trueStatement = ifelse.getTrueStatement();
	if(trueStatement)
		trueStatement->accept(*this);

	Statement* falseStatement = ifelse.getFalseStatement();
	if(falseStatement) {
		result << "\n";
		createIndent();
		result << "else ";
		falseStatement->accept(*this);
	}

	result << "\n";
}

void TreePrinter::visit(const ForStatement& forstmt)
{
	result << "for(";
	for(Argument* a: forstmt.getArguments())
		a->accept(*this);
	result << ")";
	Statement* statement = forstmt.getStatement();
	statement->accept(*this);

	result << "\n";
}

void TreePrinter::visit(const Parameter& param)
{
	result << param.getName();
	const QString& type=param.getType();
	if(!type.isEmpty())
		result << ": " << type;

	Expression* expression = param.getExpression();
	if(expression) {
		result << "=";
		expression->accept(*this);
	}
}

void TreePrinter::visit(const BinaryExpression& exp)
{
	result << "(";
	exp.getLeft()->accept(*this);
	result << exp.getOpString();
	exp.getRight()->accept(*this);
	result << ")";
}

void TreePrinter::visit(const Argument& arg)
{
	Variable* variable = arg.getVariable();
	if(variable) {
		variable->accept(*this);
		result << "=";
	}

	arg.getExpression()->accept(*this);
}

void TreePrinter::visit(const AssignStatement& stmt)
{
	Variable* var = stmt.getVariable();
	if(var)
		var->accept(*this);

	switch(stmt.getOperation()) {
		case Operators::Increment:
			result << "++";
			break;
		case Operators::Decrement:
			result << "--";
			break;
		default: {
			result << "=";
			Expression* expression = stmt.getExpression();
			if(expression)
				expression->accept(*this);
		}
	}

	result << ";\n";
}

void TreePrinter::visit(const VectorExpression& exp)
{
	result << "[";
	const QList<Expression*>& children = exp.getChildren();
	OnceOnly first;
	for(Expression* e: children) {
		if(!first())
			result << ",";
		e->accept(*this);
	}
	result << "]";
}

void TreePrinter::visit(const IntervalExpression& exp)
{
	result << to_string(exp.getValue());
	result << "[";
	exp.getMore()->accept(*this);
	Expression* less=exp.getLess();
	if(less) {
		result << ",";
		less->accept(*this);
	}
	result << "]";
}

void TreePrinter::visit(const RangeExpression& exp)
{
	result << "[";
	exp.getStart()->accept(*this);

	result << ":";
	Expression* step = exp.getStep();
	if(step) {
		step->accept(*this);
		result << ":";
	}

	exp.getFinish()->accept(*this);
	result << "]";
}

void TreePrinter::visit(const UnaryExpression& exp)
{
	const QString& op = exp.getOpString();
	const bool p = exp.postFix();
	if(!p)
		result << op;
	exp.getExpression()->accept(*this);
	if(p)
		result << op;
}

void TreePrinter::visit(const ReturnStatement& stmt)
{
	result << "return ";
	stmt.getExpression()->accept(*this);
	result << ";\n";
}

void TreePrinter::visit(const TernaryExpression& exp)
{
	result << "(";
	exp.getCondition()->accept(*this);
	result << "?";
	exp.getTrueExpression()->accept(*this);
	result << ":";
	exp.getFalseExpression()->accept(*this);
	result << ")";
}

void TreePrinter::visit(const Invocation& stmt)
{
	const QString& nameSpace = stmt.getNamespace();
	if(!nameSpace.isEmpty()) {
		result << nameSpace;
		result << "::";
	}
	result << stmt.getName();
	result << "(";
	const QList<Argument*>& arguments = stmt.getArguments();
	OnceOnly first;
	for(Argument* a: arguments) {
		if(!first())
			result << ",";
		a->accept(*this);
	}
	result << ")";
}

void TreePrinter::visit(Callback&)
{
}

void TreePrinter::printCodeDoc(const QString& desc, const QList<Parameter*>& parameters)
{
	if(!desc.isEmpty()) {
		result << "/** " << desc << "\n";
		for(Parameter* p: parameters) {
			result << " * @param " << p->getName() << " " << p->getDescription() << "\n";
		}
		result << " */\n";
	}
}

void TreePrinter::visit(const ModuleImport& decl)
{
	result << "import <";
	result << decl.getImport();
	result << ">";
	const QString& name = decl.getName();
	if(!name.isEmpty()) {
		result << " as ";
		result << name;
	}
	const QList<Parameter*>& parameters = decl.getParameters();
	const auto s=parameters.size();
	if(s>0) {
		result << "(";
		printParameters(parameters);
		result << ")";
	}
	result << ";\n";
}

void TreePrinter::visit(const ScriptImport& decl)
{
	result << "use <";
	result << decl.getImport();
	result << ">";
	const QString& name = decl.getNamespace();
	if(!name.isEmpty()) {
		result << " as ";
		result << name;
		result << ";";
	}
	result << "\n";
}

void TreePrinter::visit(const Literal& lit)
{
	result << lit.getValueString();
}

void TreePrinter::visit(const Variable& var)
{
	switch(var.getStorage()) {
		case Storage::Constant:
			result << "const ";
			break;
		case Storage::Parametric:
			result << "param ";
			break;
		default:
			break;
	}

	if(var.getStorage()==Storage::Special)
		result << "$";
	result << var.getName();
}

void TreePrinter::visit(const CodeDocParam& cd)
{
	result << cd.getName() << " " << cd.getText() << "\n";
}

void TreePrinter::visit(const CodeDocDeclaration& docs)
{
	result << "/**\n";

	for(CodeDocParam* param: docs.getParameters())
		param->accept(*this);

	result << "*/\n";
}

void TreePrinter::visit(Script& sc)
{
	for(Declaration* d: sc.getDeclarations())
		d->accept(*this);
}

void TreePrinter::visit(Product&)
{
}

void TreePrinter::visit(const ComplexExpression&)
{
}
