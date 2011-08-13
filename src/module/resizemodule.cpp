#include "resizemodule.h"
#include "vectorvalue.h"
#include "booleanvalue.h"
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

	bool autoSize=true;
	BooleanValue* autoSizeVal=dynamic_cast<BooleanValue*>(ctx->getArgument(1,"autosize"));
	if(autoSizeVal)
		autoSize=autoSizeVal->isTrue();

	ResizeNode* n=new ResizeNode();
	n->setSize(size);
	n->setChildren(childs);
	n->setAutoSize(autoSize);
	return n;
}
