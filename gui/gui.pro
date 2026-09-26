#-------------------------------------------------------------------------
#	RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
#	Copyright (C) 2010-2023 Giles Bathgate
#
#	This program is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	This program is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with this program.  If not, see <http://www.gnu.org/licenses/>.
#-------------------------------------------------------------------------

include(../common.pri)

QT  += core gui openglwidgets concurrent

TARGET = rapcad
TEMPLATE = app
INCLUDEPATH += src
INCLUDEPATH += $$clean_path($$PWD/../lib/include)
DESTDIR = $$clean_path($$OUT_PWD/..)

DEFINES += USE_READLINE

unix {
	DEFINES += DOCDIR=$$DOCDIR
}

LIBS += -L$$DESTDIR/lib -lrapcad -lvulkan
PRE_TARGETDEPS += $$DESTDIR/lib/librapcad.a

include(../dxf.pri)
include(../git.pri)
include(../cgal.pri)

win32 {
	DEFINES -= USE_READLINE
	LIBS += -lglu32
	contains(DEFINES,USE_READLINE) {
	LIBS += -lreadline
	}
} else {
	contains(DEFINES,USE_READLINE) {
	LIBS+= -lreadline
	}
  macx {
	ICON = icons/AppIcon.icns
	INCLUDEPATH += $$(BOOST_ROOT)/include
  } else {
	LIBS += -lboost_thread -lGLU
  }
}

CONFIG(fuzzing){
	QMAKE_LINK = afl-clang-fast
	QMAKE_LFLAGS += -lstdc++ -lm
	QMAKE_CC = afl-clang-fast
	QMAKE_CXX = afl-clang-fast++
}

CONFIG(test){
	QT += testlib
	DEFINES += USE_INTEGTEST
}

SOURCES += \
	src/application.cpp \
	src/headlessapplication.cpp \
	src/main.cpp \
	src/nodetreevisualiser.cpp \
	src/renderexport.cpp \
	src/ui/camera.cpp \
	src/ui/commitdialog.cpp \
	src/ui/mainwindow.cpp \
	src/syntaxhighlighter.cpp \
	src/ui/glview.cpp \
	src/ui/vkview.cpp \
	src/texteditiodevice.cpp \
	src/backgroundworker.cpp \
	src/ui/codeeditor.cpp \
	src/ui/linenumberarea.cpp \
	src/ui/preferencesdialog.cpp \
	src/ui/saveitemsdialog.cpp \
	src/ui/printconsole.cpp \
	src/project.cpp \
	src/ui/aboutdialog.cpp \
	src/tester.cpp \
	src/interactive.cpp \
	src/ui/console.cpp \
	src/ui/searchwidget.cpp

HEADERS  += \
	contrib/qtcompat.h \
	contrib/Copy_polyhedron_to.h \
	src/application.h \
	src/headlessapplication.h \
	src/nodetreevisualiser.h \
	src/renderexport.h \
	src/ui/camera.h \
	src/ui/commitdialog.h \
	src/ui/mainwindow.h \
	src/syntaxhighlighter.h \
	src/texteditiodevice.h \
	src/backgroundworker.h \
	src/ui/linenumberarea.h \
	src/ui/preferencesdialog.h \
	src/ui/saveitemsdialog.h \
	src/ui/printconsole.h \
	src/project.h \
	src/ui/aboutdialog.h \
	src/tester.h \
	src/ui/glview.h \
	src/ui/vkview.h \
	src/interactive.h \
	src/ui/codeeditor.h \
	src/ui/console.h \
	src/stringify.h \
	src/ui/searchwidget.h

FORMS += \
	src/ui/commitdialog.ui \
	src/ui/mainwindow.ui \
	src/ui/preferences.ui \
	src/ui/saveitemsdialog.ui \
	src/ui/printconsole.ui \
	src/ui/aboutdialog.ui \
	src/ui/searchwidget.ui

win32|macx {
	RESOURCES += \
	src/icons.qrc
}

win32 {
	RC_FILE = rapcad.rc
}

RESOURCES += \
	src/rapcad.qrc \
	src/ui/shaders/shaders.qrc

unix {
	target.path = $$BINDIR
	INSTALLS += target
}

# SPIR-V Shader Compilation via glslangValidator
glsl_vert.input = GLSL_VERT_SOURCES
glsl_vert.output = ${QMAKE_FILE_IN_PATH}/${QMAKE_FILE_BASE}.vert.spv
glsl_vert.commands = glslangValidator -V ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}
glsl_vert.name = GLSL Vertex Shader ${QMAKE_FILE_NAME}
glsl_vert.variable_out = PRE_TARGETDEPS
glsl_vert.CONFIG += target_predeps
QMAKE_EXTRA_COMPILERS += glsl_vert

glsl_frag.input = GLSL_FRAG_SOURCES
glsl_frag.output = ${QMAKE_FILE_IN_PATH}/${QMAKE_FILE_BASE}.frag.spv
glsl_frag.commands = glslangValidator -V ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}
glsl_frag.name = GLSL Fragment Shader ${QMAKE_FILE_NAME}
glsl_frag.variable_out = PRE_TARGETDEPS
glsl_frag.CONFIG += target_predeps
QMAKE_EXTRA_COMPILERS += glsl_frag

GLSL_VERT_SOURCES += src/ui/shaders/shader.vert
GLSL_FRAG_SOURCES += src/ui/shaders/shader.frag
