#ifndef SYNTAXTREEBUILDER_H
#define SYNTAXTREEBUILDER_H

#include <QVector>
#include "module.h"
#include "parameter.h"

class SyntaxTreeBuilder
{
public:
    SyntaxTreeBuilder();
    void StartModule(QString name,QVector<Parameter*>*);
    QVector<Parameter*>* StartParameters(Parameter*);
    Parameter* StartParameter(QString);
private:
    Module * current_module;
    QVector<Module*>  module_stack;
};

#endif // SYNTAXTREEBUILDER_H
