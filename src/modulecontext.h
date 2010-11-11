#ifndef MODULECONTEXT_H
#define MODULECONTEXT_H

#include <QVector>
#include "context.h"
#include "declaration.h"

class ModuleContext : public Context
{
public:
    ModuleContext();
    QVector<Declaration*>* getDeclarations();
    void setDeclarations(QVector<Declaration*>*);
private:
    QVector<Declaration*>* declarations;
};

#endif // MODULECONTEXT_H
