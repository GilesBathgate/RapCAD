#ifndef RESIZEMODULE_H
#define RESIZEMODULE_H

#include "module.h"

class ResizeModule : public Module
{
public:
	ResizeModule();
	Node* evaluate(Context*);
};

#endif // RESIZEMODULE_H
