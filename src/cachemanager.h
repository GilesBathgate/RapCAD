#ifndef CACHEMANAGER_H
#define CACHEMANAGER_H

#include <stddef.h>
#include "cache.h"

class CacheManager
{
public:
	static CacheManager* getInstance();
	Cache* getCache() const;
	void flushCaches();
private:
	CacheManager();
	static CacheManager* instance;

	Cache* cache;
};

#endif // CACHEMANAGER_H
