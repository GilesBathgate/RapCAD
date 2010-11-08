#include "parameter.h"

Parameter::Parameter()
{
    expression=NULL;
}

QString Parameter::toString()
{
    QString result;
    result.append("Param: ");
    result.append(this->name);
    result.append(" ");
    if(this->expression)
	result.append(this->expression->toString());
    return result;
}
