#include "export.h"

#include "application.h"
#include "cgalexport.h"
#include "renderexport.h"

Export::Export(Primitive* p,Reporter& r)
	: primitive(p),
	  reporter(r)
{
}

void Export::exportResult(const QFileInfo& fileInfo) const
{
	const QString& suffix=fileInfo.suffix().toLower();
	if(suffix=="png" || suffix=="jpg") {
		Application::headlessOverride();
		RenderExport r(primitive,reporter);
		r.exportRenderImage(fileInfo);
	} else {
#ifdef USE_CGAL
		CGALExport ce(fileInfo, primitive,reporter);
		if(suffix=="off")
			return ce.exportOFF();
		if(suffix=="obj")
			return ce.exportOBJ();
		if(suffix=="wrl")
			return ce.exportVRML();
		if(suffix=="amf")
			return ce.exportAMF();
		if(suffix=="3mf")
			return ce.export3MF();
		if(suffix=="stl")
			return ce.exportAsciiSTL();
		if(suffix=="csg")
			return ce.exportCSG();
		if(suffix=="nef")
			return ce.exportNEF();
		if(suffix=="svg")
			return ce.exportSVG();
#endif
	}
}
