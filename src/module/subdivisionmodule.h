#ifndef SUBDIVISIONMODULE_H
#define SUBDIVISIONMODULE_H

#include "module.h"

class SubDivisionModule : public Module
{
	Q_DECLARE_TR_FUNCTIONS(SubDivisionModule)
public:
	SubDivisionModule(Reporter*);
	Node* evaluate(Context*);
};

#endif // SUBDIVISIONMODULE_H
