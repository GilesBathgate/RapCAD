#include "cachemanager.h"
#include "cgalcache.h"

CacheManager* CacheManager::instance=NULL;

CacheManager* CacheManager::getInstance()
{
	if(!instance)
		instance=new CacheManager();
	return instance;
}

Cache* CacheManager::getCache() const
{
	return cache;
}

void CacheManager::flushCaches()
{
	delete cache;
	cache=createCache();
}

CacheManager::CacheManager()
{
	cache=createCache();
}

Cache*CacheManager::createCache()
{
#if USE_CGAL
	return new CGALCache();
#else
	return new Cache();
#endif
}
