#-------------------------------------------------------------------------
#    RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
#    Copyright (C) 2010-2011 Giles Bathgate
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#-------------------------------------------------------------------------

#-------------------------------------------------
#
# Project created by QtCreator 2010-10-25T09:57:37
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = rapcad
TEMPLATE = app
INCLUDEPATH += src
LEXSOURCES += src/lexer.l
YACCSOURCES += src/parser.y
LIBS += -lCGAL -lmpfr -lgmp
#LIBS += -L$$PWD/librapcad -lrapcad
QMAKE_CXXFLAGS += -frounding-math
QMAKE_YACC = bison
win32 {
QMAKE_DEL_FILE = rm -f
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
    src/evaluator.cpp \
    src/context.cpp \
    src/value.cpp \
    src/echomodule.cpp \
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
    src/cubemodule.cpp \
    src/differencemodule.cpp \
    src/polyhedronmodule.cpp \
    src/solidpython.cpp \
    src/cylindermodule.cpp \
    src/primitivemodule.cpp \
    src/node.cpp \
    src/transformationnode.cpp \
    src/operationnode.cpp \
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
    src/unionmodule.cpp \
    src/intersectionmodule.cpp \
    src/intersectionnode.cpp \
    src/translatemodule.cpp \
    src/symmetricdifferencemodule.cpp \
    src/symmetricdifferencenode.cpp \
    src/cgalprimitive.cpp \
    src/squaremodule.cpp \
    src/circlemodule.cpp \
    src/minkowskimodule.cpp \
    src/minkowskinode.cpp \
    src/rotatemodule.cpp

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
    src/echomodule.h \
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
    src/cubemodule.h \
    src/differencemodule.h \
    src/polyhedronmodule.h \
    src/solidpython.h \
    src/cylindermodule.h \
    src/primitivemodule.h \
    src/node.h \
    src/transformationnode.h \
    src/operationnode.h \
    src/GLView.h \
    src/cgalrenderer.h \
    src/OGL_helper.h \
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
    src/unionmodule.h \
    src/intersectionmodule.h \
    src/intersectionnode.h \
    src/translatemodule.h \
    src/symmetricdifferencemodule.h \
    src/symmetricdifferencenode.h \
    src/cgalprimitive.h \
    src/squaremodule.h \
    src/circlemodule.h \
    src/minkowskimodule.h \
    src/minkowskinode.h \
    src/rotatemodule.h

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
