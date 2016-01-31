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
	void disableCaches();
	void enableCaches();
private:
	CacheManager();
	static CacheManager* instance;
	Cache* createCache();
	Cache* cache;
	bool disabled;
};

#endif // CACHEMANAGER_H
