#ifndef SUBDIVISIONMODULE_H
#define SUBDIVISIONMODULE_H

#include "module.h"

class SubDivisionModule : public Module
{
public:
	SubDivisionModule(Reporter*);
	Node* evaluate(Context*);
};

#endif // SUBDIVISIONMODULE_H
