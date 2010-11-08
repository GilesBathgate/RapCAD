#include "literal.h"

Literal::Literal()
{
}

QString Literal::toString()
{
    QString result;
    result.append("Literal: ");
    QString str = QString("%1").arg(this->Value.Number);
    result.append(str);
    result.append(" ");
    return result;
}
