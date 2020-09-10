#ifndef VALUEFACTORY_H
#define VALUEFACTORY_H

#include <QList>
#include "value.h"
#include "numbervalue.h"
#include "booleanvalue.h"
#include "textvalue.h"
#include "vectorvalue.h"
#include "rangevalue.h"
#include "complexvalue.h"

class ValueFactory
{
public:
	ValueFactory();
	static ValueFactory& createFactory();
	void cleanupValues();
	void deleteValue(Value*);

	Value* createUndefined();
	BooleanValue* createBoolean(bool b);
	NumberValue* createNumber(decimal n);
	TextValue* createText(const QString&);
	VectorValue* createVector(const QList<Value*>&);
	RangeValue* createRange(Value*,Value*,Value*);
	ComplexValue* createComplex(Value*,const QList<Value*>&);
private:
	QList<Value*> values;
	static ValueFactory* factory;
};

#endif // VALUEFACTORY_H
