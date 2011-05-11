#ifndef SUBDIVISIONMODULE_H
#define SUBDIVISIONMODULE_H

#include "module.h"
#include "context.h"

class SubDivisionModule : public Module
{
public:
	SubDivisionModule();
	Node* evaluate(Context*,QList<Node*>);
};

#endif // SUBDIVISIONMODULE_H
