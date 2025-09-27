#include "gitexception.h"

GitException::GitException(const QString& message)
	: std::runtime_error(message.toUtf8())
{
}

void GitException::checkError(int errorCode)
{
	if(errorCode < 0) {
		const git_error* e=git_error_last();
		throw GitException(e && e->message ? e->message : tr("Unknown error"));
	}
}
