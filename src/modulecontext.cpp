#include "modulecontext.h"

ModuleContext::ModuleContext()
{
}

 void ModuleContext::setDeclarations(QVector<Declaration*>* decls)
 {
     this->declarations = decls;
 }

 QVector<Declaration*>* ModuleContext::getDeclarations()
 {
     return this->declarations;
 }

