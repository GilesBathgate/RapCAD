#include "resizemodule.h"
#include "context.h"
#include "node/resizenode.h"
#include "vectorvalue.h"
#include "booleanvalue.h"

ResizeModule::ResizeModule(Reporter* r) : Module(r,"resize")
{
	addDescription(tr("Modifies its children to match the given size."));
	addParameter("newsize",tr("The new size of the shape."));
	addParameter("auto",tr("Specifies whether to auto-scale any 0-dimensions"));
}

Node* ResizeModule::evaluate(Context* ctx)
{
	Point size;
	VectorValue* sizeVal=dynamic_cast<VectorValue*>(getParameterArgument(ctx,0));
	if(sizeVal)
		size=sizeVal->getPoint();

	bool autoSize=false;
	BooleanValue* autoSizeVal=dynamic_cast<BooleanValue*>(getParameterArgument(ctx,1));
	if(autoSizeVal)
		autoSize=autoSizeVal->isTrue();

	ResizeNode* n=new ResizeNode();
	n->setSize(size);
	n->setChildren(ctx->getInputNodes());
	n->setAutoSize(autoSize);
	return n;
}
