#include "cachemanager.h"
#include "cgalcache.h"
#include "emptycache.h"

CacheManager* CacheManager::instance=NULL;

CacheManager::CacheManager()
{
	disabled=true;
	cache=createCache();
}

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

void CacheManager::disableCaches()
{
	disabled=true;
	flushCaches();
}

void CacheManager::enableCaches()
{
	disabled=false;
	flushCaches();
}

Cache* CacheManager::createCache()
{
	if(disabled)
		return new EmptyCache();
#if USE_CGAL
	return new CGALCache();
#else
	return new Cache();
#endif
}
