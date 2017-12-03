#ifndef QPATHTEXTBUILDER_H
#define QPATHTEXTBUILDER_H

#include <QString>
#include <QFont>
#include <QPointF>
#include "textbuilder.h"
#include "node/polylinenode.h"
#include "reporter.h"

class QPathTextBuilder : public TextBuilder
{
public:
	QPathTextBuilder(Reporter*);
	~QPathTextBuilder() override;
	void setText(QString) override;
	void setFamily(const QString& value);
	void setSize(int value);
	decimal getHeight() override;
	void setLocation(Point) override;
	Primitive* buildPrimitive() const override;
	PrimitiveNode* buildPrimitiveNode() const;
private:
	QFont getFont() const;
	QString text;
	QString family;
	int size;
	bool headless;
	QPointF location;
	Reporter* reporter;
};

#endif // QPATHTEXTBUILDER_H
