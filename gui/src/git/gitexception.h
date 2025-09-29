#ifndef GITEXCEPTION_H
#define GITEXCEPTION_H

#include <QCoreApplication>
#include <stdexcept>
#include <QString>
#include <git2.h>

class GitException : public std::runtime_error
{
	Q_DECLARE_TR_FUNCTIONS(GitException)
public:
	explicit GitException(const QString& message);
	static void checkError(int errorCode);
};

#endif // GITEXCEPTION_H
