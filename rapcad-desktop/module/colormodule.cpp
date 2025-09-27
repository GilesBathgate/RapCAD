#include "colormodule.h"
#include "context.h"
#include "node/childrennode.h"

ColorModule::ColorModule(Reporter& r) : Module(r,"color")
{
	addDescription(tr("Sets its children to the given color."));
}

Node* ColorModule::evaluate(const Context& ctx) const
{
	reporter.reportWarning(tr("'color' module has no effect\n"));

	auto* n=new ChildrenNode();
	n->setChildren(ctx.getInputNodes());
	return n;
}
