#include "textmodule.h"
#include "textvalue.h"
#include "qpathtextbuilder.h"
#include "context.h"

TextModule::TextModule(Reporter* r) : Module(r, "text")
{
	addDescription(tr("Generates text."));
	addParameter("text",tr("The text to generate"));
}

Node* TextModule::evaluate(Context* ctx)
{
	auto* textVal=dynamic_cast<TextValue*>(getParameterArgument(ctx,0));
	if(!textVal)
		return nullptr;

	QPathTextBuilder* tb=new QPathTextBuilder(reporter);
	tb->setText(textVal->getValueString());
	PrimitiveNode* n=tb->buildPrimitiveNode();
	n->setChildren(ctx->getInputNodes());
	delete tb;
	return n;
}
