#ifndef CONFIG_H
#define CONFIG_H

#include <CGAL/config.h>
#include <qglobal.h>

#if CGAL_VERSION_NR < CGAL_VERSION_NUMBER(4,8,0)
#include "cgalassert.h" // cgalassert hack
#endif

#if CGAL_VERSION_NR > CGAL_VERSION_NUMBER(4,2,0)
#define USE_SIMPLIFY
#define USE_SUBDIV
#endif

#if __cplusplus > 199711 && CGAL_VERSION_NR > CGAL_VERSION_NUMBER(4,2,0)
#define USE_OFFSET
#endif

#endif // CONFIG_H
