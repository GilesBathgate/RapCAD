#include "valuefactory.h"

ValueFactory::ValueFactory()
{
}

ValueFactory* ValueFactory::factory=nullptr;

ValueFactory& ValueFactory::getInstance()
{
	if(!factory)
		factory = new ValueFactory();

	return *factory;
}

void ValueFactory::cleanup()
{
	delete factory;
}

Value* ValueFactory::createUndefined()
{
	auto* v = new Value();
	v->defined = false;
	values.append(v);
	return v;
}

BooleanValue* ValueFactory::createBoolean(bool b)
{
	auto* v = new BooleanValue(b);
	values.append(v);
	return v;
}

NumberValue* ValueFactory::createNumber(decimal n)
{
	auto* v = new NumberValue(n);
	values.append(v);
	return v;
}

TextValue* ValueFactory::createText(const QString& s)
{
	auto* v = new TextValue(s);
	values.append(v);
	return v;
}

VectorValue* ValueFactory::createVector(const QList<Value*>& l)
{
	auto* v = new VectorValue(l);
	values.append(v);
	return v;
}

RangeValue* ValueFactory::createRange(Value* start,Value* step,Value* finish)
{
	auto* v = new RangeValue(start,step,finish);
	values.append(v);
	return v;
}

ComplexValue* ValueFactory::createComplex(Value* r,const QList<Value*>& i)
{
	auto* v = new ComplexValue(r,i);
	values.append(v);
	return v;
}

void ValueFactory::cleanupValues()
{
	foreach(Value* val, values) {
		delete val;
	}
}

void ValueFactory::deleteValue(Value* v)
{
	values.removeAll(v);
}
