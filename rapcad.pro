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

#-------------------------------------------------
#
# Project created by QtCreator 2010-10-25T09:57:37
#
#-------------------------------------------------
VERSION = $$cat(VERSION)

QT  += core gui openglwidgets concurrent

CONFIG += c++17
TARGET = rapcad
TEMPLATE = app
INCLUDEPATH += src

DEFINES += USE_CGAL
DEFINES += USE_READLINE

win32 {
	DEFINES -= USE_READLINE

	DXFLIBROOT = ../dxflib-3.3.4-src
	INCLUDEPATH += $$(CGAL_DIR)/include
	INCLUDEPATH += $$(CGAL_DIR)/auxiliary/gmp/include
	INCLUDEPATH += $$(BOOST_ROOT)
	LIBS += -L$$(CGAL_DIR)/lib
	exists( $$(CGAL_DIR)/lib/libCGAL* ) {
		LIBS +=  -lCGAL -lCGAL_Core
	}
	LIBS += -L$$(CGAL_DIR)/auxiliary/gmp/lib -lmpfr-4 -lgmp-10
	LIBS += -lglu32
	contains(DEFINES,USE_READLINE) {
	LIBS += -lreadline
	}
	contains(DEFINES,USE_DXF) {
	INCLUDEPATH += $$DXFLIBROOT
	LIBS += -L$$DXFLIBROOT/release -ldxflib
	}
} else {

	exists( /usr/lib/x86_64-linux-gnu/libCGAL* ) {
		LIBS += -lCGAL -lCGAL_Core
	}
	exists( /usr/lib/i386-linux-gnu/libCGAL* ) {
		LIBS += -lCGAL -lCGAL_Core
	}
	LIBS += -lmpfr -lgmp -lgmpxx
	contains(DEFINES,USE_READLINE) {
	LIBS+= -lreadline
	}
	contains(DEFINES,USE_DXF) {
	LIBS += -ldxflib
	}
  macx {
	ICON = icons/AppIcon.icns
	INCLUDEPATH += $$(CGAL_DIR)/include
	INCLUDEPATH += $$(GMP)/include
	INCLUDEPATH += $$(MPFR)/include
	INCLUDEPATH += $$(BOOST_ROOT)/include
	LIBS += -L$$(GMP)/lib
	LIBS += -L$$(MPFR)/lib
	exists( /usr/local/lib/libCGAL* ) {
		LIBS += -lCGAL -lCGAL_Core
	}
  } else {
	LIBS += -lboost_thread -lGLU
  }
}

# Configure yacc/lex
win32 {
	QMAKE_YACC = win_bison
	QMAKE_YACCFLAGS += "-b y"
	QMAKE_LEX = win_flex
} else {
	QMAKE_YACC = bison
	QMAKE_LEX = flex
}

BISON_VERSION = $$system($$QMAKE_YACC --version)
BISON_VERSION = $$find(BISON_VERSION, [0-9]+.[0-9]+.[0-9]+)
BISON_VERSIONS = $$split(BISON_VERSION, ".")
BISON_MAJOR_VERSION = $$member(BISON_VERSIONS, 0)
BISON_MINOR_VERSION = $$member(BISON_VERSIONS, 1)

# Check for Bison Version 3.6 and above
# (so Major > 2 && Minor > 5)
greaterThan(BISON_MAJOR_VERSION, 2) {
	greaterThan(BISON_MINOR_VERSION, 5) | greaterThan(BISON_MAJOR_VERSION, 3) {
		QMAKE_YACCFLAGS += "-D api.header.include={\\\"parser_yacc.h\\\"}"
	}
}

#LIBS += -Wl,-rpath,./librapcad -L./librapcad -lrapcad

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

CONFIG(coverage){
	QT += testlib
	DEFINES += USE_INTEGTEST
	CONFIG += debug
  !macx {
	QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
	LIBS += -lgcov
  }
}

CONFIG(official){
	DEFINES += RAPCAD_VERSION=$$VERSION
} else {
	MAJOR=$$system(cut -d'.' -f1 VERSION)
	MINOR=$$system(cut -d'.' -f2 VERSION)
	DEFINES += RAPCAD_VERSION=$$MAJOR"."$$MINOR".git."$$system(git log -1 --pretty=format:%h)
}

LEXSOURCES += \
	src/lexer.l

YACCSOURCES += \
	src/parser.y

SOURCES += \
	src/application.cpp \
	src/assertexception.cpp \
	src/builtinmanager.cpp \
	src/cgalauxiliarybuilder.cpp \
	src/cgaldiscretemodifier.cpp \
	src/cgalgroupmodifier.cpp \
	src/cgalsanitizer.cpp \
	src/codedocdeclaration.cpp \
	src/function/assertfunction.cpp \
	src/function/circumcenterfunction.cpp \
	src/function/isundeffunction.cpp \
	src/function/ordinalfunction.cpp \
	src/geometryevaluator.cpp \
	src/intervalexpression.cpp \
	src/intervalvalue.cpp \
	src/main.cpp \
	src/module/assertmodule.cpp \
	src/module/colormodule.cpp \
	src/module/cuboidmodule.cpp \
	src/module/datummodule.cpp \
	src/module/regularpolygonmodule.cpp \
	src/module/solidmodule.cpp \
	src/namedvalue.cpp \
	src/node/solidnode.cpp \
	src/tokenreader.cpp \
	src/ui/camera.cpp \
	src/ui/mainwindow.cpp \
	src/module.cpp \
	src/syntaxtreebuilder.cpp \
	src/parameter.cpp \
	src/expression.cpp \
	src/binaryexpression.cpp \
	src/literal.cpp \
	src/valuefactory.cpp \
	src/variable.cpp \
	src/declaration.cpp \
	src/scope.cpp \
	src/modulescope.cpp \
	src/dependencybuilder.cpp \
	src/instance.cpp \
	src/argument.cpp \
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
	src/ui/glview.cpp \
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
	src/module/spheremodule.cpp \
	src/worker.cpp \
	src/reporter.cpp \
	src/codedocparam.cpp \
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
	src/preferences.cpp \
	src/cgalexport.cpp \
	src/module/prismmodule.cpp \
	src/function/sqrtfunction.cpp \
	src/function/sumfunction.cpp \
	src/function/randfunction.cpp \
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
	src/function/versionfunction.cpp \
	src/module/polygonmodule.cpp \
	src/function/lengthfunction.cpp \
	src/ui/printconsole.cpp \
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
	src/ui/aboutdialog.cpp \
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
	src/module/multmatrixmodule.cpp \
	src/polygon.cpp \
	src/onceonly.cpp \
	src/fragment.cpp \
	src/cgalfragment.cpp \
	src/function/concatfunction.cpp \
	src/node/groupnode.cpp \
	src/decimal.cpp \
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
	src/function/numfunction.cpp \
	src/rmath.cpp \
	src/module/triangulatemodule.cpp \
	src/node/triangulatenode.cpp \
	src/function/normfunction.cpp \
	src/complexexpression.cpp \
	src/complexvalue.cpp \
	src/function/angfunction.cpp \
	src/transformmatrix.cpp \
	src/module/materialmodule.cpp \
	src/node/materialnode.cpp \
	src/simplerenderer.cpp \
	src/module/discretemodule.cpp \
	src/node/discretenode.cpp \
	src/cachemanager.cpp \
	src/cache.cpp \
	src/cgalcache.cpp \
	src/function/crossfunction.cpp \
	src/module/childrenmodule.cpp \
	src/node/normalsnode.cpp \
	src/module/normalsmodule.cpp \
	src/module/simplifymodule.cpp \
	src/node/simplifynode.cpp \
	src/module/chainhullmodule.cpp \
	src/function/isnumfunction.cpp \
	src/function/isboolfunction.cpp \
	src/function/isstrfunction.cpp \
	src/function/islistfunction.cpp \
	src/function/israngefunction.cpp \
	src/function/isintfunction.cpp \
	src/function/chrfunction.cpp \
	src/textiterator.cpp \
	src/node/childrennode.cpp \
	src/function/ismat4x4function.cpp \
	src/asciidocprinter.cpp \
	src/generator.cpp \
	src/qpathtextbuilder.cpp \
	src/module/textmodule.cpp \
	src/module/boundarymodule.cpp \
	src/node/boundarynode.cpp \
	src/function/isvecfunction.cpp \
	src/node/pointsnode.cpp \
	src/module/pointsmodule.cpp \
	src/cgalprojection.cpp \
	src/function/cbrtfunction.cpp \
	src/ui/searchwidget.cpp

HEADERS  += \
	contrib/fragments.h \
	contrib/mpfr-get_q.h \
	contrib/mpfr-impl.h \
	contrib/Copy_polyhedron_to.h \
	contrib/qtcompat.h \
	src/abstractsettings.h \
	src/application.h \
	src/assertexception.h \
	src/bedappearance.h \
	src/builtinmanager.h \
	src/cgalauxiliarybuilder.h \
	src/cgaldiscretemodifier.h \
	src/cgalgroupmodifier.h \
	src/cgalsanitizer.h \
	src/cgaltrace.h \
	src/codedocdeclaration.h \
	src/function/assertfunction.h \
	src/function/circumcenterfunction.h \
	src/function/isundeffunction.h \
	src/function/ordinalfunction.h \
	src/geometryevaluator.h \
	src/intervalexpression.h \
	src/intervalvalue.h \
	src/module/assertmodule.h \
	src/module/colormodule.h \
	src/module/cuboidmodule.h \
	src/module/datummodule.h \
	src/module/regularpolygonmodule.h \
	src/module/solidmodule.h \
	src/namedvalue.h \
	src/node/solidnode.h \
	src/operators.h \
	src/tokenreader.h \
	src/ui/camera.h \
	src/ui/mainwindow.h \
	src/module.h \
	src/syntaxtreebuilder.h \
	src/parameter.h \
	src/expression.h \
	src/binaryexpression.h \
	src/literal.h \
	src/valuefactory.h \
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
	src/viewdirections.h \
	src/visitabletree.h \
	src/script.h \
	src/syntaxhighlighter.h \
	src/tokenbuilder.h \
	src/abstracttokenbuilder.h \
	src/treeevaluator.h \
	src/context.h \
	src/value.h \
	src/module/echomodule.h \
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
	src/cgalrenderer.h \
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
	src/module/spheremodule.h \
	src/worker.h \
	src/reporter.h \
	src/codedocparam.h \
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
	src/preferences.h \
	src/cgalexport.h \
	src/module/prismmodule.h \
	src/function/sqrtfunction.h \
	src/function/sumfunction.h \
	src/function/randfunction.h \
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
	src/function/versionfunction.h \
	src/module/polygonmodule.h \
	src/function/lengthfunction.h \
	src/ui/printconsole.h \
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
	src/ui/aboutdialog.h \
	src/module/pointsmodule.h \
	src/node/pointsnode.h \
	src/module/slicemodule.h \
	src/node/slicenode.h \
	src/primitive.h \
	src/module/conemodule.h \
	src/function/lnfunction.h \
	src/function/logfunction.h \
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
	src/module/multmatrixmodule.h \
	src/onceonly.h \
	src/ui/glview.h \
	src/fragment.h \
	src/cgalfragment.h \
	src/function/concatfunction.h \
	src/node/groupnode.h \
	src/decimal.h \
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
	src/function/numfunction.h \
	src/rmath.h \
	src/module/triangulatemodule.h \
	src/node/triangulatenode.h \
	src/function/normfunction.h \
	src/complexexpression.h \
	src/complexvalue.h \
	src/function/angfunction.h \
	src/transformmatrix.h \
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
	src/function/crossfunction.h \
	src/module/childrenmodule.h \
	src/node/normalsnode.h \
	src/module/normalsmodule.h \
	src/module/simplifymodule.h \
	src/node/simplifynode.h \
	contrib/qzipreader_p.h \
	contrib/qzipwriter_p.h \
	src/module/chainhullmodule.h \
	src/function/isnumfunction.h \
	src/function/isboolfunction.h \
	src/function/isstrfunction.h \
	src/function/islistfunction.h \
	src/function/israngefunction.h \
	src/function/isintfunction.h \
	src/function/chrfunction.h \
	src/textiterator.h \
	src/node/childrennode.h \
	src/function/ismat4x4function.h \
	src/asciidocprinter.h \
	src/generator.h \
	src/qpathtextbuilder.h \
	src/module/textmodule.h \
	src/module/boundarymodule.h \
	src/node/boundarynode.h \
	src/config.h \
	src/function/isvecfunction.h \
	src/cgalprojection.h \
	src/function/cbrtfunction.h \
	src/ui/searchwidget.h

FORMS += \
	src/ui/mainwindow.ui \
	src/ui/preferences.ui \
	src/ui/saveitemsdialog.ui \
	src/ui/printconsole.ui \
	src/ui/aboutdialog.ui \
	src/ui/searchwidget.ui

OTHER_FILES += \
	COPYING

userguide.target = user_guide.html
userguide.depends = $$PWD/doc/user_guide.asciidoc

win32 {
	userguide.commands = python $$(ASCIIDOC)\\asciidoc.py -o $$userguide.target $$userguide.depends
} else {
	userguide.commands = asciidoc -o $$userguide.target $$userguide.depends
}

QMAKE_EXTRA_TARGETS += userguide

unix {
	isEmpty(PREFIX) {
		PREFIX = /usr
	}
	isEmpty(BINDIR) {
		BINDIR = $$PREFIX/bin
	}
	isEmpty(DATAROOTDIR) {
		DATAROOTDIR=$$PREFIX/share
	}
	isEmpty(DOCDIR) {
		DOCDIR=$$DATAROOTDIR/doc/rapcad
	}
	target.path = $$BINDIR

	docs.depends = $$userguide.target
	docs.files = $$userguide.target
	docs.path = $$DOCDIR

	DEFINES += DOCDIR=$$DOCDIR

	INSTALLS += \
		target \
		docs
}

win32|macx {
	RESOURCES += \
	src/icons.qrc
}

win32 {
	RC_FILE = rapcad.rc
}

RESOURCES += \
src/rapcad.qrc
