#include "valuefactory.h"
#include <QtGlobal>

ValueFactory::~ValueFactory()
{
	cleanupValues();
}

ValueFactory& ValueFactory::getInstance()
{
	static ValueFactory factory;
	return factory;
}

void ValueFactory::appendValue(Value* v)
{
	getInstance().values.append(v);
}

Value& ValueFactory::createUndefined()
{
	auto* v = new Value();
	v->defined = false;
	appendValue(v);
	return *v;
}

BooleanValue& ValueFactory::createBoolean(bool b)
{
	auto* v = new BooleanValue(b);
	appendValue(v);
	return *v;
}

NumberValue& ValueFactory::createNumber(const decimal& n)
{
	auto* v = new NumberValue(n);
	appendValue(v);
	return *v;
}

TextValue& ValueFactory::createText(const QString& s)
{
	auto* v = new TextValue(s);
	appendValue(v);
	return *v;
}

VectorValue& ValueFactory::createVector(const QList<Value*>& l)
{
	auto* v = new VectorValue(l);
	appendValue(v);
	return *v;
}

RangeValue& ValueFactory::createRange(Value& start,Value& finish)
{
	auto* v = new RangeValue(start,finish);
	appendValue(v);
	return *v;
}

RangeValue& ValueFactory::createRange(Value& start,Value& step,Value& finish)
{
	auto* v = new RangeValue(start,step,finish);
	appendValue(v);
	return *v;
}

ComplexValue& ValueFactory::createComplex(Value& r,const QList<Value*>& i)
{
	auto* v = new ComplexValue(r,i);
	appendValue(v);
	return *v;
}

void ValueFactory::cleanupValues()
{
	qDeleteAll(values);
	values.clear();
}

void ValueFactory::deleteValue(Value* v)
{
#if QT_VERSION >= QT_VERSION_CHECK(4,4,0)
	values.removeOne(v);
#else
	values.removeAll(v);
#endif
}
