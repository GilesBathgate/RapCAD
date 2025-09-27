#include "signature.h"
#include "gitexception.h"

Signature::Signature(const Repository& r)
{
	const auto error=git_signature_default(&signature, r.repo);
	GitException::checkError(error);
}

Signature::~Signature()
{
	git_signature_free(signature);
}
