#-------------------------------------------------------------------------
#	RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
#	Copyright (C) 2010-2011 Giles Bathgate
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

QT	+= core gui opengl

TARGET = rapcad
TEMPLATE = app
LEXSOURCES += src/lexer.l
YACCSOURCES += src/parser.y
INCLUDEPATH += src
win32{
INCLUDEPATH += "..\\CGAL-3.7\\include"
INCLUDEPATH += "..\\CGAL-3.7\\auxiliary\\gmp\\include"
INCLUDEPATH += "..\\dxflib-2.2.0.0-1.src"
INCLUDEPATH += "..\\boost_1_46_1"
LIBS += -L"..\\boost_1_46_1\\bin.v2\\libs\\thread\\build\\gcc-mingw-4.5.2\\release\\threading-multi"
LIBS += -llibboost_thread-mgw45-mt-1_46_1
LIBS += -L"..\\CGAL-3.7\\lib" -lCGAL -lCGAL_Core
LIBS += -L"..\\CGAL-3.7\\auxiliary\\gmp\\lib" -lmpfr-4 -lgmp-10
LIBS += -L"..\\dxflib-2.2.0.0-1.src\\lib" -llibdxf
QMAKE_YACC = "..\\MinGW\\msys\\1.0\\bin\\bison"
QMAKE_YACCFLAGS += "-b y"
QMAKE_LEX = "..\\MinGW\\msys\\1.0\\bin\\flex"
QMAKE_MOVE = "..\\MinGW\\msys\\1.0\\bin\\mv"
}
unix{
LIBS += -lCGAL -lCGAL_Core -lmpfr -lgmp -ldxflib
QMAKE_YACC = bison
}
#LIBS += -L$$PWD/librapcad -lrapcad
QMAKE_CXXFLAGS += -frounding-math

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
	src/evaluator.cpp \
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
	src/primitivenode.cpp \
	src/module/cubemodule.cpp \
	src/module/differencemodule.cpp \
	src/module/polyhedronmodule.cpp \
	src/module/cylindermodule.cpp \
	src/module/primitivemodule.cpp \
	src/node.cpp \
	src/transformationnode.cpp \
	src/glview.cpp \
	src/cgalrenderer.cpp \
	src/point.cpp \
	src/nodeprinter.cpp \
	src/nodeevaluator.cpp \
	src/texteditiodevice.cpp \
	src/backgroundworker.cpp \
	src/cgalbuilder.cpp \
	src/differencenode.cpp \
	src/unionnode.cpp \
	src/module/unionmodule.cpp \
	src/module/intersectionmodule.cpp \
	src/intersectionnode.cpp \
	src/module/translatemodule.cpp \
	src/module/symmetricdifferencemodule.cpp \
	src/symmetricdifferencenode.cpp \
	src/cgalprimitive.cpp \
	src/module/squaremodule.cpp \
	src/module/circlemodule.cpp \
	src/module/minkowskimodule.cpp \
	src/minkowskinode.cpp \
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
	src/hullnode.cpp \
	src/module/linearextrudemodule.cpp \
	src/linearextrudenode.cpp \
	src/module/boundsmodule.cpp \
	src/boundsnode.cpp \
	src/module/subdivisionmodule.cpp \
	src/subdivisionnode.cpp \
	src/module/insetmodule.cpp \
	src/insetnode.cpp \
	src/module/polylinemodule.cpp \
	src/polylinenode.cpp \
	src/module/glidemodule.cpp \
	src/glidenode.cpp \
	src/cgalpolygon.cpp


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
	src/evaluator.h \
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
	src/primitivenode.h \
	src/module/cubemodule.h \
	src/module/differencemodule.h \
	src/module/polyhedronmodule.h \
	src/module/cylindermodule.h \
	src/module/primitivemodule.h \
	src/node.h \
	src/transformationnode.h \
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
	src/differencenode.h \
	src/unionnode.h \
	src/module/unionmodule.h \
	src/module/intersectionmodule.h \
	src/intersectionnode.h \
	src/module/translatemodule.h \
	src/module/symmetricdifferencemodule.h \
	src/symmetricdifferencenode.h \
	src/cgalprimitive.h \
	src/module/squaremodule.h \
	src/module/circlemodule.h \
	src/module/minkowskimodule.h \
	src/minkowskinode.h \
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
	src/hullnode.h \
	src/module/linearextrudemodule.h \
	src/linearextrudenode.h \
	src/module/boundsmodule.h \
	src/boundsnode.h \
	src/module/subdivisionmodule.h \
	src/subdivisionnode.h \
	src/module/insetmodule.h \
	src/insetnode.h \
	src/module/polylinemodule.h \
	src/polylinenode.h \
	src/module/glidemodule.h \
	src/glidenode.h \
	src/cgalpolygon.h

FORMS += \
	src/mainwindow.ui

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
