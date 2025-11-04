#ifndef RENDEREXPORT_H
#define RENDEREXPORT_H

#include "export.h"
#include "renderer.h"

#include <QObject>
#include <QOffscreenSurface>
#include <QOpenGLFunctions_2_0>
#include <QOpenGLContext>
#include <QMatrix4x4>

class RenderExport : public QOffscreenSurface, public Export, private QOpenGLFunctions_2_0
{
	Q_OBJECT
public:
	RenderExport(Primitive*,Reporter&,QObject* parent=nullptr);
	void exportResult(const QFileInfo&) override;
private:
	void initializeGL();
	void resizeGL(int w, int h);
	Renderer* getRenderer() const;

	QOpenGLContext context;
	QMatrix4x4 projection;
	QMatrix4x4 modelview;
};

#endif // RENDEREXPORT_H
