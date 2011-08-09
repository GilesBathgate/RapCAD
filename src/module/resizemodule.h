#ifndef RESIZEMODULE_H
#define RESIZEMODULE_H

#include "module.h"
#include "context.h"

class ResizeModule : public Module
{
public:
	ResizeModule();
	Node* evaluate(Context*,QList<Node*>);
};

#endif // RESIZEMODULE_H
