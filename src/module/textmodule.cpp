#include "textmodule.h"
#include "textvalue.h"
#include "numbervalue.h"
#include "qpathtextbuilder.h"
#include "context.h"

TextModule::TextModule(Reporter* r) : Module(r, "text")
{
	addDescription(tr("Generates text."));
	addParameter("text",tr("The text to generate"));
	addParameter("font",tr("The font of the text"));
	addParameter("size",tr("The size of the text"));
}

Node* TextModule::evaluate(Context* ctx)
{
	auto* textVal=dynamic_cast<TextValue*>(getParameterArgument(ctx,0));
	if(!textVal)
		return nullptr;

	QString family;
	auto* fontVal=dynamic_cast<TextValue*>(getParameterArgument(ctx,1));
	if(fontVal)
		family=fontVal->getValueString();

	int size=12;
	auto* sizeVal=dynamic_cast<NumberValue*>(getParameterArgument(ctx,2));
	if(sizeVal)
		size=sizeVal->toInteger();

	QPathTextBuilder* tb=new QPathTextBuilder(reporter);
	tb->setText(textVal->getValueString());
	tb->setFamily(family);
	tb->setSize(size);

	PrimitiveNode* n=tb->buildPrimitiveNode();
	n->setSanitized(false);
	n->setChildren(ctx->getInputNodes());
	delete tb;
	return n;
}
