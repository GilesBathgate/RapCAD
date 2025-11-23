#ifndef SIGNATURE_H
#define SIGNATURE_H

#include <git2.h>
#include "repository.h"

class Signature
{
public:
	Signature(const Repository&);
	~Signature();
private:
	friend class Repository;
	git_signature* signature;
};

#endif // SIGNATURE_H
