#ifndef VALUEFACTORY_H
#define VALUEFACTORY_H

#include "booleanvalue.h"
#include "complexvalue.h"
#include "intervalvalue.h"
#include "numbervalue.h"
#include "rangevalue.h"
#include "textvalue.h"
#include "value.h"
#include "vectorvalue.h"
#include <QList>

class ValueFactory
{
	Q_DISABLE_COPY_MOVE(ValueFactory)
public:
	static ValueFactory& getInstance();

	void cleanupValues();
	void deleteValue(Value*);

	static Value& createUndefined();
	static BooleanValue& createBoolean(bool b);
	static NumberValue& createNumber(const decimal&);
	static TextValue& createText(const QString&);
	static VectorValue& createVector(const QList<Value*>&);
	static RangeValue& createRange(Value&,Value&);
	static RangeValue& createRange(Value&,Value&,Value&);
	static ComplexValue& createComplex(Value&,const QList<Value*>&);
	static IntervalValue& createInterval(Value&,Value&);
private:
	ValueFactory() = default;
	~ValueFactory();
	static void appendValue(Value*);
	QList<Value*> values;
};

#endif // VALUEFACTORY_H
