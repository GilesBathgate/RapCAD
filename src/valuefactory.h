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
	static ValueFactory& getInstance();
	static void cleanup();

	void cleanupValues();
	void deleteValue(Value*);

	Value& createUndefined();
	BooleanValue& createBoolean(bool b);
	NumberValue& createNumber(const decimal&);
	TextValue& createText(const QString&);
	VectorValue& createVector(const QList<Value*>&);
	RangeValue& createRange(Value&,Value&);
	RangeValue& createRange(Value&,Value&,Value&);
	ComplexValue& createComplex(Value&,const QList<Value*>&);

private:
	ValueFactory() = default;
	~ValueFactory();
	QList<Value*> values;
};

#endif // VALUEFACTORY_H
