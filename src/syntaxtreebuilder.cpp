#include "syntaxtreebuilder.h"

SyntaxTreeBuilder::SyntaxTreeBuilder()
{
}

void SyntaxTreeBuilder::StartModule(QString name, QVector<Parameter*>* params)
{
    current_module = new Module();
    current_module->name = name;
    current_module->parameters = params;
}

QVector<Parameter*>* SyntaxTreeBuilder::StartParameters(Parameter* param)
{
    QVector<Parameter*>* result = new QVector<Parameter*>();
    result->append(param);
    return result;
}

Parameter* SyntaxTreeBuilder::StartParameter(QString name)
{
    Parameter* result = new Parameter();
    result->name = name;
    return result;
}
