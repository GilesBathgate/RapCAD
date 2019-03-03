/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2019 Giles Bathgate
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "cachemanager.h"
#include "cgalcache.h"
#include "emptycache.h"

CacheManager* CacheManager::instance=nullptr;

CacheManager::CacheManager() :
	disabled(true)
{
	cache=createCache();
}

CacheManager::~CacheManager()
{
	delete cache;
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
#ifdef USE_CGAL
	return new CGALCache();
#else
	return new Cache();
#endif
}
