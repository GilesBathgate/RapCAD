#include "renderexport.h"

#include "preferences.h"
#include "ui/camera.h"

#ifdef USE_CGAL
#include "cgalrenderer.h"
#else
#include "simplerenderer.h"
#endif

#include <QApplication>
#include <QOpenGLFramebufferObject>

static const GLfloat farfarAway=100000.0F;

RenderExport::RenderExport(Primitive* p,Reporter& r)
	: Export(p,r)
{
}

Renderer* RenderExport::getRenderer() const
{
#ifdef USE_CGAL
	try {
		return new CGALRenderer(reporter,*primitive);
	} catch(const CGAL::Failure_exception& e) {
		return nullptr;
	}
#else
	return new SimpleRenderer(*primitive);
#endif
}

void RenderExport::initializeGL()
{
	initializeOpenGLFunctions();

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0F,1.0F,1.0F,0.0F);
	GLfloat light_diffuse[]={1.0F,1.0F,1.0F,1.0F};
	GLfloat light_position0[]={-1.0F,-1.0F,+1.0F,0.0F};
	GLfloat light_position1[]={+1.0F,+1.0F,-1.0F,0.0F};

	glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
	glLightfv(GL_LIGHT0,GL_POSITION,light_position0);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,light_diffuse);
	glLightfv(GL_LIGHT1,GL_POSITION,light_position1);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
}

void RenderExport::resizeGL(int w,int h)
{
	glViewport(0,0,static_cast<GLint>(w),static_cast<GLint>(h));

	glMatrixMode(GL_PROJECTION);

	projection.setToIdentity();
	projection.perspective(45.0F,static_cast<GLfloat>(w)/static_cast<GLfloat>(h),+10.0F,+farfarAway);
	glLoadMatrixf(projection.data());

}

void RenderExport::exportRenderImage(const QFileInfo& fileInfo)
{
	const auto width = 640;
	const auto height = 480;

	create();
	context.setFormat(requestedFormat());
	context.create();
	context.makeCurrent(this);
	initializeGL();
	resizeGL(width,height);

	const auto& p=Preferences::getInstance();
	const Camera camera{
		p.getDefaultRotationX(),
		p.getDefaultRotationY(),
		p.getDefaultRotationZ(),
		p.getDefaultX(),
		p.getDefaultDistance(),
		p.getDefaultZ()
	};

	QOpenGLFramebufferObjectFormat format;
	format.setAttachment(QOpenGLFramebufferObject::Depth);
	QOpenGLFramebufferObject frameBuffer(width,height,format);
	frameBuffer.bind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	modelview.setToIdentity();
	camera.applyTo(modelview);
	glLoadMatrixf(modelview.data());

	auto renderer=getRenderer();
	if (renderer)
		renderer->paint(*this, false, false);
	delete renderer;

	frameBuffer.release();

	const QImage& image = frameBuffer.toImage();
	image.save(fileInfo.absoluteFilePath());
	context.doneCurrent();
	destroy();
}
