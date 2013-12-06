#-------------------------------------------------------------------------
#	RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
#	Copyright (C) 2010-2013 Giles Bathgate
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

#-------------------------------------------------
#
# Project created by QtCreator 2010-10-25T09:57:37
#
#-------------------------------------------------
VERSION = $$cat(VERSION)

QT  += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
}

TARGET = rapcad
TEMPLATE = app
LEXSOURCES += src/lexer.l
YACCSOURCES += src/parser.y
INCLUDEPATH += src

win32 {
	CGALROOT = "..\\CGAL-4.0\\"
	BOOSTROOT = "..\\boost_1_49_0\\"
	DXFLIBROOT = "..\\dxflib-2.2.0.0-1.src\\"
	MINGWROOT = "..\\MinGW\\msys\\1.0\\bin\\"
	INCLUDEPATH += $$CGALROOT"include"
	INCLUDEPATH += $$CGALROOT"auxiliary\\gmp\\include"
	INCLUDEPATH += $$DXFLIBROOT
	INCLUDEPATH += $$BOOSTROOT
	LIBS += -L$$BOOSTROOT"bin.v2\\libs\\thread\\build\\gcc-mingw-4.6.2\\release\\threading-multi"
	LIBS += -llibboost_thread-mgw46-mt-1_49
	LIBS += -L$$CGALROOT"lib" -lCGAL -lCGAL_Core
	LIBS += -L$$CGALROOT"auxiliary\\gmp\\lib" -lmpfr-4 -lgmp-10
	LIBS += -L$$DXFLIBROOT"lib" -llibdxf
	QMAKE_YACC = $$MINGWROOT"bison"
	QMAKE_YACCFLAGS += "-b y"
	QMAKE_LEX = $$MINGWROOT"flex"
	QMAKE_MOVE = $$MINGWROOT"mv"
	QMAKE_DEL_FILE = $$MINGWROOT"rm -f"
} else {
	LIBS += -lCGAL -lCGAL_Core -lmpfr -lgmp -ldxflib
	QMAKE_YACC = bison
  macx {
	INCLUDEPATH += /opt/local/include
	LIBS += -L/opt/local/lib -lboost_thread-mt
	QMAKE_MOC = $$[QT_INSTALL_BINS]\\moc -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED
  } else {
	LIBS += -lboost_thread -lGLU
  }
}

#LIBS += -Wl,-rpath,./librapcad -L./librapcad -lrapcad

DEFINES+=USE_CGAL
QMAKE_CXXFLAGS += -frounding-math

CONFIG(official){
	DEFINES += RAPCAD_VERSION=$$VERSION
} else {
	MAJOR=$$system(cut -d'.' -f1 VERSION)
	MINOR=$$system(cut -d'.' -f2 VERSION)
	DEFINES += RAPCAD_VERSION=$$MAJOR"."$$MINOR".git."$$system(git log -1 --pretty=format:%h)
}

SOURCES += \
	src/main.cpp \
	src/mainwindow.cpp \
	src/module.cpp \
	src/syntaxtreebuilder.cpp \
	src/parameter.cpp \
	src/expression.cpp \
	src/binaryexpression.cpp \
	src/literal.cpp \
	src/variable.cpp \
	src/declaration.cpp \
	src/scope.cpp \
	src/modulescope.cpp \
	src/dependencybuilder.cpp \
	src/instance.cpp \
	src/argument.cpp \
	src/statement.cpp \
	src/function.cpp \
	src/functionscope.cpp \
	src/compoundstatement.cpp \
	src/assignstatement.cpp \
	src/vectorexpression.cpp \
	src/ifelsestatement.cpp \
	src/forstatement.cpp \
	src/rangeexpression.cpp \
	src/unaryexpression.cpp \
	src/invocation.cpp \
	src/returnstatement.cpp \
	src/ternaryexpression.cpp \
	src/moduleimport.cpp \
	src/treeprinter.cpp \
	src/script.cpp \
	src/syntaxhighlighter.cpp \
	src/tokenbuilder.cpp \
	src/treeevaluator.cpp \
	src/context.cpp \
	src/value.cpp \
	src/module/echomodule.cpp \
	src/numbervalue.cpp \
	src/booleanvalue.cpp \
	src/textvalue.cpp \
	src/vectorvalue.cpp \
	src/rangevalue.cpp \
	src/valueiterator.cpp \
	src/vectoriterator.cpp \
	src/rangeiterator.cpp \
	src/scriptimport.cpp \
	src/node/primitivenode.cpp \
	src/module/cubemodule.cpp \
	src/module/differencemodule.cpp \
	src/module/polyhedronmodule.cpp \
	src/module/cylindermodule.cpp \
	src/module/primitivemodule.cpp \
	src/node.cpp \
	src/node/transformationnode.cpp \
	src/glview.cpp \
	src/cgalrenderer.cpp \
	src/point.cpp \
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
	src/module/childmodule.cpp \
	src/module/spheremodule.cpp \
	src/worker.cpp \
	src/reporter.cpp \
	src/codedoc.cpp \
	src/dxfbuilder.cpp \
	src/module/shearmodule.cpp \
	src/module/groupmodule.cpp \
	src/codeeditor.cpp \
	src/linenumberarea.cpp \
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
	src/module/polylinemodule.cpp \
	src/node/polylinenode.cpp \
	src/module/glidemodule.cpp \
	src/node/glidenode.cpp \
	src/cgalpolygon.cpp \
	src/module/beziersurfacemodule.cpp \
	src/preferencesdialog.cpp \
	src/preferences.cpp \
	src/cgalexport.cpp \
	src/module/prismmodule.cpp \
	src/function/sqrtfunction.cpp \
	src/function/sumfunction.cpp \
	src/function/randfunction.cpp \
	src/module/cylindersurfacemodule.cpp \
	src/module/outlinemodule.cpp \
	src/node/outlinenode.cpp \
	src/module/importmodule.cpp \
	src/builtincreator.cpp \
	src/node/importnode.cpp \
	src/cgalimport.cpp \
	src/module/resizemodule.cpp \
	src/node/resizenode.cpp \
	src/module/rotateextrudemodule.cpp \
	src/node/rotateextrudenode.cpp \
	src/saveitemsdialog.cpp \
	src/function/versionfunction.cpp \
	src/module/polygonmodule.cpp \
	src/function/lengthfunction.cpp \
	src/printconsole.cpp \
	src/function/strfunction.cpp \
	src/project.cpp \
	src/function/sinfunction.cpp \
	src/function/cosfunction.cpp \
	src/function/tanfunction.cpp \
	src/function/absfunction.cpp \
	src/function/signfunction.cpp \
	src/function/minfunction.cpp \
	src/function/maxfunction.cpp \
	src/function/roundfunction.cpp \
	src/function/ceilfunction.cpp \
	src/function/floorfunction.cpp \
	src/function/powfunction.cpp \
	src/function/expfunction.cpp \
	src/function/asinfunction.cpp \
	src/function/acosfunction.cpp \
	src/function/atan2function.cpp \
	src/function/atanfunction.cpp \
	src/function/coshfunction.cpp \
	src/function/sinhfunction.cpp \
	src/function/tanhfunction.cpp \
	src/module/centermodule.cpp \
	src/node/centernode.cpp \
	src/aboutdialog.cpp \
	src/module/pointmodule.cpp \
	src/node/pointnode.cpp \
	src/module/slicemodule.cpp \
	src/node/slicenode.cpp \
	src/module/conemodule.cpp \
	src/function/lnfunction.cpp \
	src/function/logfunction.cpp \
	src/module/writemodule.cpp \
	src/module/writelnmodule.cpp \
	src/callback.cpp \
	src/product.cpp \
	src/node/productnode.cpp \
	src/function/radfunction.cpp \
	src/function/degfunction.cpp \
	src/layout.cpp \
	src/module/projectionmodule.cpp \
	src/node/projectionnode.cpp \
	src/tester.cpp \
	src/strategy.cpp \
	src/comparer.cpp \
	src/module/multmatrixmodule.cpp

HEADERS  += \
	src/mainwindow.h \
	src/module.h \
	src/syntaxtreebuilder.h \
	src/parameter.h \
	src/expression.h \
	src/binaryexpression.h \
	src/literal.h \
	src/variable.h \
	src/declaration.h \
	src/scope.h \
	src/modulescope.h \
	src/abstractsyntaxtreebuilder.h \
	src/dependencybuilder.h \
	src/instance.h \
	src/argument.h \
	src/statement.h \
	src/function.h \
	src/functionscope.h \
	src/compoundstatement.h \
	src/assignstatement.h \
	src/vectorexpression.h \
	src/ifelsestatement.h \
	src/forstatement.h \
	src/rangeexpression.h \
	src/unaryexpression.h \
	src/invocation.h \
	src/returnstatement.h \
	src/ternaryexpression.h \
	src/moduleimport.h \
	src/treevisitor.h \
	src/treeprinter.h \
	src/visitabletree.h \
	src/script.h \
	src/syntaxhighlighter.h \
	src/tokenbuilder.h \
	src/abstracttokenbuilder.h \
	src/treeevaluator.h \
	src/context.h \
	src/value.h \
	src/module/echomodule.h \
	src/tau.h \
	src/numbervalue.h \
	src/booleanvalue.h \
	src/textvalue.h \
	src/vectorvalue.h \
	src/rangevalue.h \
	src/iterator.h \
	src/valueiterator.h \
	src/vectoriterator.h \
	src/rangeiterator.h \
	src/scriptimport.h \
	src/cgal.h \
	src/node/primitivenode.h \
	src/module/cubemodule.h \
	src/module/differencemodule.h \
	src/module/polyhedronmodule.h \
	src/module/cylindermodule.h \
	src/module/primitivemodule.h \
	src/node.h \
	src/node/transformationnode.h \
	src/GLView.h \
	src/cgalrenderer.h \
	contrib/OGL_helper.h \
	src/renderer.h \
	src/point.h \
	src/nodevisitor.h \
	src/visitablenode.h \
	src/nodeprinter.h \
	src/nodeevaluator.h \
	src/polygon.h \
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
	src/module/childmodule.h \
	src/module/spheremodule.h \
	src/worker.h \
	src/reporter.h \
	src/codedoc.h \
	src/dxfbuilder.h \
	src/module/shearmodule.h \
	src/module/groupmodule.h \
	src/CodeEditor.h \
	src/linenumberarea.h \
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
	src/module/polylinemodule.h \
	src/node/polylinenode.h \
	src/module/glidemodule.h \
	src/node/glidenode.h \
	src/cgalpolygon.h \
	src/module/beziersurfacemodule.h \
	src/preferencesdialog.h \
	src/preferences.h \
	src/cgalexport.h \
	src/module/prismmodule.h \
	src/function/sqrtfunction.h \
	src/function/sumfunction.h \
	src/function/randfunction.h \
	src/module/cylindersurfacemodule.h \
	src/module/outlinemodule.h \
	src/node/outlinenode.h \
	src/module/importmodule.h \
	src/builtincreator.h \
	src/node/importnode.h \
	src/cgalimport.h \
	src/module/resizemodule.h \
	src/node/resizenode.h \
	src/module/rotateextrudemodule.h \
	src/node/rotateextrudenode.h \
	src/saveitemsdialog.h \
	src/function/versionfunction.h \
	src/module/polygonmodule.h \
	src/function/lengthfunction.h \
	src/printconsole.h \
	src/function/strfunction.h \
	src/project.h \
	src/function/sinfunction.h \
	src/function/cosfunction.h \
	src/function/tanfunction.h \
	src/function/absfunction.h \
	src/function/signfunction.h \
	src/function/minfunction.h \
	src/function/maxfunction.h \
	src/function/roundfunction.h \
	src/function/ceilfunction.h \
	src/function/floorfunction.h \
	src/function/powfunction.h \
	src/function/expfunction.h \
	src/function/asinfunction.h \
	src/function/acosfunction.h \
	src/function/atan2function.h \
	src/function/atanfunction.h \
	src/function/coshfunction.h \
	src/function/sinhfunction.h \
	src/function/tanhfunction.h \
	src/module/centermodule.h \
	src/node/centernode.h \
	src/aboutdialog.h \
	src/module/pointmodule.h \
	src/node/pointnode.h \
	src/module/slicemodule.h \
	src/node/slicenode.h \
	src/primitive.h \
	src/module/conemodule.h \
	src/function/lnfunction.h \
	src/function/logfunction.h \
	src/nullprimitive.h \
	src/module/writemodule.h \
	src/module/writelnmodule.h \
	src/callback.h \
	src/product.h \
	src/node/productnode.h \
	src/function/radfunction.h \
	src/function/degfunction.h \
	src/layout.h \
	src/module/projectionmodule.h \
	src/node/projectionnode.h \
	src/tester.h \
	src/cgalassert.h \
	src/strategy.h \
	src/comparer.h \
	src/module/multmatrixmodule.h

FORMS += \
	src/mainwindow.ui \
	src/preferences.ui \
	src/saveitemsdialog.ui \
	src/printconsole.ui \
	src/aboutdialog.ui

OTHER_FILES += \
	COPYING

unix {
	isEmpty(PREFIX) {
		PREFIX = /usr
	}
	BINDIR = $$PREFIX/bin
	INSTALLS += target
	target.path =$$BINDIR
}

win32|macx {
	RESOURCES += \
	src/icons.qrc
}

RESOURCES += \
src/rapcad.qrc
