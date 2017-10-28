#ifndef TEXTMODULE_H
#define TEXTMODULE_H

#include "module.h"

class TextModule : public Module
{
	Q_DECLARE_TR_FUNCTIONS(TextModule)
public:
	TextModule(Reporter*);
	Node* evaluate(Context*) override;
};

#endif // TEXTMODULE_H
