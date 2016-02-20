#ifndef RESIZEMODULE_H
#define RESIZEMODULE_H

#include "module.h"

class ResizeModule : public Module
{
public:
	ResizeModule(Reporter*);
	Node* evaluate(Context*);
};

#endif // RESIZEMODULE_H
