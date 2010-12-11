#-------------------------------------------------------------------------
#    RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
#    Copyright (C) 2010  Giles Bathgate
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
QMAKE_YACC = bison
QMAKE_DEL_FILE = rm -f
LEXSOURCES += src/lexer.l
YACCSOURCES += src/parser.y

SOURCES += src/main.cpp\
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
    src/prettyprinter.cpp \
    src/script.cpp \
    src/syntaxhighlighter.cpp \
    src/tokenbuilder.cpp \
    src/evaluator.cpp \
    src/context.cpp \
    src/value.cpp

HEADERS  += src/mainwindow.h \
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
    src/visitor.h \
    src/prettyprinter.h \
    src/visitable.h \
    src/script.h \
    src/syntaxhighlighter.h \
    src/tokenbuilder.h \
    src/abstracttokenbuilder.h \
    src/evaluator.h \
    src/context.h \
    src/value.h

FORMS    += src/mainwindow.ui   

OTHER_FILES += \
    COPYING
