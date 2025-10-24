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
INCLUDEPATH += $$clean_path($$PWD/../lib/src)
DESTDIR = $$clean_path($$OUT_PWD/..)

DEFINES += USE_READLINE

unix {
	DEFINES += DOCDIR=$$DOCDIR
}

LIBS += -L$$DESTDIR/lib -lrapcad
PRE_TARGETDEPS += $$DESTDIR/lib/librapcad.a

include(../git.pri)
include(../cgal.pri)

win32 {
	DEFINES -= USE_READLINE
	DXFLIBROOT = ../dxflib-3.3.4-src
	LIBS += -lglu32
	contains(DEFINES,USE_READLINE) {
	LIBS += -lreadline
	}
	contains(DEFINES,USE_DXF) {
	INCLUDEPATH += $$DXFLIBROOT
	LIBS += -L$$DXFLIBROOT/release -ldxflib
	}
} else {
	contains(DEFINES,USE_READLINE) {
	LIBS+= -lreadline
	}
	contains(DEFINES,USE_DXF) {
	LIBS += -ldxflib
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

CONFIG(valgrind){
	DEFINES += USE_VALGRIND
	DEFINES -= CGAL_USE_GMPXX
	QMAKE_CXXFLAGS += -fno-rounding-math -fdebug-default-version=4
} else:!macx {
	QMAKE_CXXFLAGS += -frounding-math
}

CONFIG(test){
	QT += testlib
	DEFINES += USE_INTEGTEST
}

SOURCES += \
	src/application.cpp \
	src/builtinmanager.cpp \
	src/cgalauxiliarybuilder.cpp \
	src/cgaldiscretemodifier.cpp \
	src/cgalgroupmodifier.cpp \
	src/cgalsanitizer.cpp \
	src/export.cpp \
	src/geometryevaluator.cpp \
	src/main.cpp \
	src/module/assertmodule.cpp \
	src/module/colormodule.cpp \
	src/module/cuboidmodule.cpp \
	src/module/datummodule.cpp \
	src/module/regularpolygonmodule.cpp \
	src/module/solidmodule.cpp \
	src/module/threadmodule.cpp \
	src/node/solidnode.cpp \
	src/renderexport.cpp \
	src/ui/camera.cpp \
	src/ui/commitdialog.cpp \
	src/ui/mainwindow.cpp \
	src/dependencybuilder.cpp \
	src/syntaxhighlighter.cpp \
	src/treeevaluator.cpp \
	src/module/echomodule.cpp \
	src/node/primitivenode.cpp \
	src/module/cubemodule.cpp \
	src/module/differencemodule.cpp \
	src/module/polyhedronmodule.cpp \
	src/module/cylindermodule.cpp \
	src/module/primitivemodule.cpp \
	src/node/transformationnode.cpp \
	src/ui/glview.cpp \
	src/cgalrenderer.cpp \
	src/nodeprinter.cpp \
	src/nodeevaluator.cpp \
	src/texteditiodevice.cpp \
	src/backgroundworker.cpp \
	src/cgalbuilder.cpp \
	src/node/differencenode.cpp \
	src/node/unionnode.cpp \
	src/module/unionmodule.cpp \
	src/module/intersectionmodule.cpp \
	src/node/intersectionnode.cpp \
	src/module/translatemodule.cpp \
	src/module/symmetricdifferencemodule.cpp \
	src/node/symmetricdifferencenode.cpp \
	src/cgalprimitive.cpp \
	src/module/squaremodule.cpp \
	src/module/circlemodule.cpp \
	src/module/minkowskimodule.cpp \
	src/node/minkowskinode.cpp \
	src/module/rotatemodule.cpp \
	src/module/mirrormodule.cpp \
	src/module/scalemodule.cpp \
	src/module/spheremodule.cpp \
	src/worker.cpp \
	src/dxfbuilder.cpp \
	src/module/shearmodule.cpp \
	src/module/groupmodule.cpp \
	src/ui/codeeditor.cpp \
	src/ui/linenumberarea.cpp \
	src/cgalexplorer.cpp \
	src/module/hullmodule.cpp \
	src/node/hullnode.cpp \
	src/module/linearextrudemodule.cpp \
	src/node/linearextrudenode.cpp \
	src/module/boundsmodule.cpp \
	src/node/boundsnode.cpp \
	src/module/subdivisionmodule.cpp \
	src/node/subdivisionnode.cpp \
	src/module/offsetmodule.cpp \
	src/node/offsetnode.cpp \
	src/module/glidemodule.cpp \
	src/node/glidenode.cpp \
	src/cgalpolygon.cpp \
	src/module/beziersurfacemodule.cpp \
	src/ui/preferencesdialog.cpp \
	src/cgalexport.cpp \
	src/module/prismmodule.cpp \
	src/module/cylindersurfacemodule.cpp \
	src/module/importmodule.cpp \
	src/builtincreator.cpp \
	src/node/importnode.cpp \
	src/cgalimport.cpp \
	src/module/resizemodule.cpp \
	src/node/resizenode.cpp \
	src/module/rotateextrudemodule.cpp \
	src/node/rotateextrudenode.cpp \
	src/ui/saveitemsdialog.cpp \
	src/module/polygonmodule.cpp \
	src/ui/printconsole.cpp \
	src/project.cpp \
	src/module/centermodule.cpp \
	src/ui/aboutdialog.cpp \
	src/module/slicemodule.cpp \
	src/node/slicenode.cpp \
	src/module/conemodule.cpp \
	src/module/writemodule.cpp \
	src/module/writelnmodule.cpp \
	src/module/projectionmodule.cpp \
	src/node/projectionnode.cpp \
	src/tester.cpp \
	src/strategy.cpp \
	src/comparer.cpp \
	src/module/multmatrixmodule.cpp \
	src/node/groupnode.cpp \
	src/polyhedron.cpp \
	src/module/decomposemodule.cpp \
	src/node/decomposenode.cpp \
	src/simpletextbuilder.cpp \
	src/interactive.cpp \
	src/ui/console.cpp \
	src/module/alignmodule.cpp \
	src/node/alignnode.cpp \
	src/module/complementmodule.cpp \
	src/node/complementnode.cpp \
	src/module/radialsmodule.cpp \
	src/node/radialsnode.cpp \
	src/module/volumesmodule.cpp \
	src/node/volumesnode.cpp \
	src/cgalvolume.cpp \
	src/module/triangulatemodule.cpp \
	src/node/triangulatenode.cpp \
	src/module/materialmodule.cpp \
	src/node/materialnode.cpp \
	src/simplerenderer.cpp \
	src/module/discretemodule.cpp \
	src/node/discretenode.cpp \
	src/cachemanager.cpp \
	src/cache.cpp \
	src/cgalcache.cpp \
	src/module/childrenmodule.cpp \
	src/node/normalsnode.cpp \
	src/module/normalsmodule.cpp \
	src/module/simplifymodule.cpp \
	src/node/simplifynode.cpp \
	src/module/chainhullmodule.cpp \
	src/node/childrennode.cpp \
	src/generator.cpp \
	src/qpathtextbuilder.cpp \
	src/module/textmodule.cpp \
	src/module/boundarymodule.cpp \
	src/node/boundarynode.cpp \
	src/node/pointsnode.cpp \
	src/module/pointsmodule.cpp \
	src/cgalprojection.cpp \
	src/ui/searchwidget.cpp

HEADERS  += \
	contrib/qtcompat.h \
	contrib/Copy_polyhedron_to.h \
	src/application.h \
	src/builtinmanager.h \
	src/cgalauxiliarybuilder.h \
	src/cgaldiscretemodifier.h \
	src/cgalgroupmodifier.h \
	src/cgalsanitizer.h \
	src/export.h \
	src/geometryevaluator.h \
	src/module/assertmodule.h \
	src/module/colormodule.h \
	src/module/cuboidmodule.h \
	src/module/datummodule.h \
	src/module/regularpolygonmodule.h \
	src/module/solidmodule.h \
	src/module/threadmodule.h \
	src/node/solidnode.h \
	src/renderexport.h \
	src/ui/camera.h \
	src/ui/commitdialog.h \
	src/ui/mainwindow.h \
	src/dependencybuilder.h \
	src/syntaxhighlighter.h \
	src/treeevaluator.h \
	src/module/echomodule.h \
	src/node/primitivenode.h \
	src/module/cubemodule.h \
	src/module/differencemodule.h \
	src/module/polyhedronmodule.h \
	src/module/cylindermodule.h \
	src/module/primitivemodule.h \
	src/node/transformationnode.h \
	src/cgalrenderer.h \
	src/renderer.h \
	src/nodeprinter.h \
	src/nodeevaluator.h \
	src/texteditiodevice.h \
	src/backgroundworker.h \
	src/cgalbuilder.h \
	src/node/differencenode.h \
	src/node/unionnode.h \
	src/module/unionmodule.h \
	src/module/intersectionmodule.h \
	src/node/intersectionnode.h \
	src/module/translatemodule.h \
	src/module/symmetricdifferencemodule.h \
	src/node/symmetricdifferencenode.h \
	src/cgalprimitive.h \
	src/module/squaremodule.h \
	src/module/circlemodule.h \
	src/module/minkowskimodule.h \
	src/node/minkowskinode.h \
	src/module/rotatemodule.h \
	src/module/mirrormodule.h \
	src/module/scalemodule.h \
	src/module/spheremodule.h \
	src/worker.h \
	src/dxfbuilder.h \
	src/module/shearmodule.h \
	src/module/groupmodule.h \
	src/ui/linenumberarea.h \
	src/cgalexplorer.h \
	src/module/hullmodule.h \
	src/node/hullnode.h \
	src/module/linearextrudemodule.h \
	src/node/linearextrudenode.h \
	src/module/boundsmodule.h \
	src/node/boundsnode.h \
	src/module/subdivisionmodule.h \
	src/node/subdivisionnode.h \
	src/module/offsetmodule.h \
	src/node/offsetnode.h \
	src/module/glidemodule.h \
	src/node/glidenode.h \
	src/cgalpolygon.h \
	src/module/beziersurfacemodule.h \
	src/ui/preferencesdialog.h \
	src/cgalexport.h \
	src/module/prismmodule.h \
	src/module/cylindersurfacemodule.h \
	src/module/importmodule.h \
	src/builtincreator.h \
	src/node/importnode.h \
	src/cgalimport.h \
	src/module/resizemodule.h \
	src/node/resizenode.h \
	src/module/rotateextrudemodule.h \
	src/node/rotateextrudenode.h \
	src/ui/saveitemsdialog.h \
	src/module/polygonmodule.h \
	src/ui/printconsole.h \
	src/project.h \
	src/module/centermodule.h \
	src/ui/aboutdialog.h \
	src/module/pointsmodule.h \
	src/node/pointsnode.h \
	src/module/slicemodule.h \
	src/node/slicenode.h \
	src/module/conemodule.h \
	src/module/writemodule.h \
	src/module/writelnmodule.h \
	src/module/projectionmodule.h \
	src/node/projectionnode.h \
	src/tester.h \
	src/strategy.h \
	src/comparer.h \
	src/module/multmatrixmodule.h \
	src/ui/glview.h \
	src/node/groupnode.h \
	src/polyhedron.h \
	src/module/decomposemodule.h \
	src/node/decomposenode.h \
	src/textbuilder.h \
	src/simpletextbuilder.h \
	src/interactive.h \
	src/ui/codeeditor.h \
	src/ui/console.h \
	src/module/alignmodule.h \
	src/node/alignnode.h \
	src/module/complementmodule.h \
	src/node/complementnode.h \
	src/module/radialsmodule.h \
	src/node/radialsnode.h \
	src/module/volumesmodule.h \
	src/node/volumesnode.h \
	src/cgalvolume.h \
	src/module/triangulatemodule.h \
	src/node/triangulatenode.h \
	src/module/materialmodule.h \
	src/node/materialnode.h \
	src/simplerenderer.h \
	src/module/discretemodule.h \
	src/node/discretenode.h \
	src/stringify.h \
	src/cachemanager.h \
	src/cache.h \
	src/cgalcache.h \
	src/emptycache.h \
	src/module/childrenmodule.h \
	src/node/normalsnode.h \
	src/module/normalsmodule.h \
	src/module/simplifymodule.h \
	src/node/simplifynode.h \
	contrib/qzipreader_p.h \
	contrib/qzipwriter_p.h \
	src/module/chainhullmodule.h \
	src/node/childrennode.h \
	src/generator.h \
	src/qpathtextbuilder.h \
	src/module/textmodule.h \
	src/module/boundarymodule.h \
	src/node/boundarynode.h \
	src/cgalprojection.h \
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
	src/rapcad.qrc

unix {
	target.path = $$BINDIR
	INSTALLS += target
}
