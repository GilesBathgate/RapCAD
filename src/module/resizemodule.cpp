#include "resizemodule.h"
#include "node/resizenode.h"
#include "vectorvalue.h"
#include "booleanvalue.h"

ResizeModule::ResizeModule() : Module("resize")
{
	addParameter("size");
	addParameter("autosize");
}

Node* ResizeModule::evaluate(Context* ctx)
{
	Point size;
	VectorValue* sizeVal=dynamic_cast<VectorValue*>(getParameterArgument(ctx,0));
	if(sizeVal)
		size=sizeVal->getPoint();

	bool autoSize=true;
	BooleanValue* autoSizeVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,1));
	if(autoSizeVal)
		autoSize=autoSizeVal->isTrue();

	ResizeNode* n=new ResizeNode();
	n->setSize(size);
	n->setChildren(ctx->getInputNodes());
	n->setAutoSize(autoSize);
	return n;
}
