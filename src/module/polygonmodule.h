#ifndef POLYGONMODULE_H
#define POLYGONMODULE_H

#include "module.h"
#include "context.h"

class PolygonModule : public Module
{
public:
	PolygonModule();
	Node* evaluate(Context*,QList<Node*>);
};

#endif // POLYGONMODULE_H
