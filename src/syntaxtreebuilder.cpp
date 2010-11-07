#include "syntaxtreebuilder.h"

SyntaxTreeBuilder::SyntaxTreeBuilder()
{
}

void SyntaxTreeBuilder::BuildModule(QString name, QVector<Parameter*>* params)
{
    current_module = new Module();
    current_module->name = name;
    current_module->parameters = params;
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

Parameter* SyntaxTreeBuilder::BuildParameter(QString name)
{
    Parameter* result = new Parameter();
    result->name = name;
    return result;
}

Parameter* SyntaxTreeBuilder::BuildParameter(QString name,Expression* expr)
{
    Parameter* result = new Parameter();
    result->name = name;
    result->expression = expr;
    return result;
}

Expression* SyntaxTreeBuilder::BuildExpression(bool value)
{
    return new Expression();
}

Expression* SyntaxTreeBuilder::BuildExpression(double value)
{
    return new Expression();
}

Expression* SyntaxTreeBuilder::BuildExpression(Expression* left ,Expression* right)
{
    BinaryExpression* result = new BinaryExpression();
    result->left = left;
    result->right = right;
    return result;
}
