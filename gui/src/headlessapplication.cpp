#include "headlessapplication.h"
#include <QApplication>

HeadlessApplication::HeadlessApplication()
{
	setInstance();
}

Headless& HeadlessApplication::create()
{
	static HeadlessApplication instance;
	return instance;
}

QCoreApplication* HeadlessApplication::headlessOverride() const
{
	int c=0;
	static auto* instance {
		QApplication::instance() ?: new QApplication(c,nullptr)
	};
	return instance;
}
