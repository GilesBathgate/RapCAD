#include "module.h"

Module::Module()
{
}

QString Module::toString()
{
    QString result;
    result.append("Module: ");
    result.append(this->name);
    result.append("\n");
    result.append("Parameters: ");
    QVector<Parameter*>* params = this->parameters;
    for(int i=0; i<params->size(); i++)
	result.append(params->at(i)->toString());

    return result;
}
