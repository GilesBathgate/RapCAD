#include "resizemodule.h"
#include "vectorvalue.h"
#include "resizenode.h"

ResizeModule::ResizeModule() : Module("resize")
{
}

Node* ResizeModule::evaluate(Context* ctx, QList<Node *> childs)
{
	Point size;
	VectorValue* sizeVal=dynamic_cast<VectorValue*>(ctx->getArgument(0,"size"));
	if(sizeVal)
		size=sizeVal->getPoint();

	ResizeNode* n=new ResizeNode();
	n->setSize(size);
	n->setChildren(childs);
	return n;
}
