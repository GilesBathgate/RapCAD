#ifndef RENDEREXPORT_H
#define RENDEREXPORT_H

#include "export.h"
#include "renderer.h"

#include <QOffscreenSurface>
#include <QOpenGLFunctions_2_0>
#include <QOpenGLContext>
#include <QMatrix4x4>

class RenderExport : public Export, public QOffscreenSurface, private QOpenGLFunctions_2_0
{
public:
	RenderExport(Primitive*,Reporter&);
	void exportRenderImage(const QFileInfo&);
private:
	void initializeGL();
	void resizeGL(int w, int h);
	Renderer* getRenderer() const;

	QOpenGLContext context;
	QMatrix4x4 projection;
	QMatrix4x4 modelview;
};

#endif // RENDEREXPORT_H
