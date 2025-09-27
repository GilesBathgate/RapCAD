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
VERSION = $$cat(../VERSION)

QT  += core gui openglwidgets concurrent

CONFIG += c++17
TARGET = rapcad
TEMPLATE = app

DEFINES += USE_CGAL
DEFINES += USE_READLINE

win32 {
	DEFINES -= USE_READLINE

	DXFLIBROOT = ../dxflib-3.3.4-src
	INCLUDEPATH += $$(CGAL_DIR)/include
	INCLUDEPATH += $$(CGAL_DIR)/auxiliary/gmp/include
	INCLUDEPATH += $$(BOOST_ROOT)
	INCLUDEPATH += $$(LIBGIT2_DIR)/include
	LIBS += -L$$(CGAL_DIR)/lib
	exists( $$(CGAL_DIR)/lib/libCGAL* ) {
		LIBS +=  -lCGAL -lCGAL_Core
	}
	LIBS += -L$$(CGAL_DIR)/auxiliary/gmp/lib -lmpfr-4 -lgmp-10
	LIBS += -L$$(LIBGIT2_DIR)/lib -lgit2
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
	LIBS += -lgit2
  macx {
	ICON = icons/AppIcon.icns
	INCLUDEPATH += $$(CGAL_DIR)/include
	INCLUDEPATH += $$(GMP)/include
	INCLUDEPATH += $$(MPFR)/include
	INCLUDEPATH += $$(BOOST_ROOT)/include
	INCLUDEPATH += $$(LIBGIT2_DIR)/include
	LIBS += -L$$(GMP)/lib
	LIBS += -L$$(MPFR)/lib
	exists( /usr/local/lib/libCGAL* ) {
		LIBS += -lCGAL -lCGAL_Core
	}
	LIBS += -L$$(LIBGIT2_DIR)/lib -lgit2
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
	DEFINES += DISABLE_MULTITHREAD_TESTS
	DEFINES += DISABLE_UI_TESTS
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
	MAJOR=$$system(cut -d'.' -f1 ../VERSION)
	MINOR=$$system(cut -d'.' -f2 ../VERSION)
	DEFINES += RAPCAD_VERSION=$$MAJOR"."$$MINOR".git."$$system(git log -1 --pretty=format:%h)
}

LEXSOURCES += \
        lexer.l

YACCSOURCES += \
        parser.y

SOURCES += \
	application.cpp \
	assertexception.cpp \
	builtinmanager.cpp \
	cgalauxiliarybuilder.cpp \
	cgaldiscretemodifier.cpp \
	cgalgroupmodifier.cpp \
	cgalsanitizer.cpp \
	codedocdeclaration.cpp \
	export.cpp \
	function/assertfunction.cpp \
	function/circumcenterfunction.cpp \
	function/fragmentsfunction.cpp \
	function/isundeffunction.cpp \
	function/ordinalfunction.cpp \
	geometryevaluator.cpp \
	git/commit.cpp \
	git/gitexception.cpp \
	git/index.cpp \
	git/oid.cpp \
	git/repository.cpp \
	git/repositorymanager.cpp \
	git/signature.cpp \
	git/tree.cpp \
	intervalexpression.cpp \
	intervalvalue.cpp \
	main.cpp \
	module/assertmodule.cpp \
	module/colormodule.cpp \
	module/cuboidmodule.cpp \
	module/datummodule.cpp \
	module/regularpolygonmodule.cpp \
	module/solidmodule.cpp \
	module/threadmodule.cpp \
	namedvalue.cpp \
	node/solidnode.cpp \
	renderexport.cpp \
	tokenreader.cpp \
	ui/camera.cpp \
	ui/commitdialog.cpp \
	ui/mainwindow.cpp \
	module.cpp \
	syntaxtreebuilder.cpp \
	parameter.cpp \
	expression.cpp \
	binaryexpression.cpp \
	literal.cpp \
	valuefactory.cpp \
	variable.cpp \
	declaration.cpp \
	scope.cpp \
	modulescope.cpp \
	dependencybuilder.cpp \
	instance.cpp \
	argument.cpp \
	function.cpp \
	functionscope.cpp \
	compoundstatement.cpp \
	assignstatement.cpp \
	vectorexpression.cpp \
	ifelsestatement.cpp \
	forstatement.cpp \
	rangeexpression.cpp \
	unaryexpression.cpp \
	invocation.cpp \
	returnstatement.cpp \
	ternaryexpression.cpp \
	moduleimport.cpp \
	treeprinter.cpp \
	script.cpp \
	syntaxhighlighter.cpp \
	tokenbuilder.cpp \
	treeevaluator.cpp \
	context.cpp \
	value.cpp \
	module/echomodule.cpp \
	numbervalue.cpp \
	booleanvalue.cpp \
	textvalue.cpp \
	vectorvalue.cpp \
	rangevalue.cpp \
	valueiterator.cpp \
	vectoriterator.cpp \
	rangeiterator.cpp \
	scriptimport.cpp \
	node/primitivenode.cpp \
	module/cubemodule.cpp \
	module/differencemodule.cpp \
	module/polyhedronmodule.cpp \
	module/cylindermodule.cpp \
	module/primitivemodule.cpp \
	node.cpp \
	node/transformationnode.cpp \
	ui/glview.cpp \
	cgalrenderer.cpp \
	point.cpp \
	nodeprinter.cpp \
	nodeevaluator.cpp \
	texteditiodevice.cpp \
	backgroundworker.cpp \
	cgalbuilder.cpp \
	node/differencenode.cpp \
	node/unionnode.cpp \
	module/unionmodule.cpp \
	module/intersectionmodule.cpp \
	node/intersectionnode.cpp \
	module/translatemodule.cpp \
	module/symmetricdifferencemodule.cpp \
	node/symmetricdifferencenode.cpp \
	cgalprimitive.cpp \
	module/squaremodule.cpp \
	module/circlemodule.cpp \
	module/minkowskimodule.cpp \
	node/minkowskinode.cpp \
	module/rotatemodule.cpp \
	module/mirrormodule.cpp \
	module/scalemodule.cpp \
	module/spheremodule.cpp \
	worker.cpp \
	reporter.cpp \
	codedocparam.cpp \
	dxfbuilder.cpp \
	module/shearmodule.cpp \
	module/groupmodule.cpp \
	ui/codeeditor.cpp \
	ui/linenumberarea.cpp \
	cgalexplorer.cpp \
	module/hullmodule.cpp \
	node/hullnode.cpp \
	module/linearextrudemodule.cpp \
	node/linearextrudenode.cpp \
	module/boundsmodule.cpp \
	node/boundsnode.cpp \
	module/subdivisionmodule.cpp \
	node/subdivisionnode.cpp \
	module/offsetmodule.cpp \
	node/offsetnode.cpp \
	module/glidemodule.cpp \
	node/glidenode.cpp \
	cgalpolygon.cpp \
	module/beziersurfacemodule.cpp \
	ui/preferencesdialog.cpp \
	preferences.cpp \
	cgalexport.cpp \
	module/prismmodule.cpp \
	function/sqrtfunction.cpp \
	function/sumfunction.cpp \
	function/randfunction.cpp \
	module/cylindersurfacemodule.cpp \
	module/importmodule.cpp \
	builtincreator.cpp \
	node/importnode.cpp \
	cgalimport.cpp \
	module/resizemodule.cpp \
	node/resizenode.cpp \
	module/rotateextrudemodule.cpp \
	node/rotateextrudenode.cpp \
	ui/saveitemsdialog.cpp \
	function/versionfunction.cpp \
	module/polygonmodule.cpp \
	function/lengthfunction.cpp \
	ui/printconsole.cpp \
	function/strfunction.cpp \
	project.cpp \
	function/sinfunction.cpp \
	function/cosfunction.cpp \
	function/tanfunction.cpp \
	function/absfunction.cpp \
	function/signfunction.cpp \
	function/minfunction.cpp \
	function/maxfunction.cpp \
	function/roundfunction.cpp \
	function/ceilfunction.cpp \
	function/floorfunction.cpp \
	function/powfunction.cpp \
	function/expfunction.cpp \
	function/asinfunction.cpp \
	function/acosfunction.cpp \
	function/atan2function.cpp \
	function/atanfunction.cpp \
	function/coshfunction.cpp \
	function/sinhfunction.cpp \
	function/tanhfunction.cpp \
	module/centermodule.cpp \
	ui/aboutdialog.cpp \
	module/slicemodule.cpp \
	node/slicenode.cpp \
	module/conemodule.cpp \
	function/lnfunction.cpp \
	function/logfunction.cpp \
	module/writemodule.cpp \
	module/writelnmodule.cpp \
	callback.cpp \
	product.cpp \
	node/productnode.cpp \
	function/radfunction.cpp \
	function/degfunction.cpp \
	layout.cpp \
	module/projectionmodule.cpp \
	node/projectionnode.cpp \
	tester.cpp \
	strategy.cpp \
	comparer.cpp \
	module/multmatrixmodule.cpp \
	polygon.cpp \
	onceonly.cpp \
	fragment.cpp \
	cgalfragment.cpp \
	function/concatfunction.cpp \
	node/groupnode.cpp \
	decimal.cpp \
	polyhedron.cpp \
	module/decomposemodule.cpp \
	node/decomposenode.cpp \
	simpletextbuilder.cpp \
	interactive.cpp \
	ui/console.cpp \
	module/alignmodule.cpp \
	node/alignnode.cpp \
	module/complementmodule.cpp \
	node/complementnode.cpp \
	module/radialsmodule.cpp \
	node/radialsnode.cpp \
	module/volumesmodule.cpp \
	node/volumesnode.cpp \
	cgalvolume.cpp \
	function/numfunction.cpp \
	rmath.cpp \
	module/triangulatemodule.cpp \
	node/triangulatenode.cpp \
	function/normfunction.cpp \
	complexexpression.cpp \
	complexvalue.cpp \
	function/angfunction.cpp \
	transformmatrix.cpp \
	module/materialmodule.cpp \
	node/materialnode.cpp \
	simplerenderer.cpp \
	module/discretemodule.cpp \
	node/discretenode.cpp \
	cachemanager.cpp \
	cache.cpp \
	cgalcache.cpp \
	function/crossfunction.cpp \
	module/childrenmodule.cpp \
	node/normalsnode.cpp \
	module/normalsmodule.cpp \
	module/simplifymodule.cpp \
	node/simplifynode.cpp \
	module/chainhullmodule.cpp \
	function/isnumfunction.cpp \
	function/isboolfunction.cpp \
	function/isstrfunction.cpp \
	function/islistfunction.cpp \
	function/israngefunction.cpp \
	function/isintfunction.cpp \
	function/chrfunction.cpp \
	textiterator.cpp \
	node/childrennode.cpp \
	function/ismat4x4function.cpp \
	asciidocprinter.cpp \
	generator.cpp \
	qpathtextbuilder.cpp \
	module/textmodule.cpp \
	module/boundarymodule.cpp \
	node/boundarynode.cpp \
	function/isvecfunction.cpp \
	node/pointsnode.cpp \
	module/pointsmodule.cpp \
	cgalprojection.cpp \
	function/cbrtfunction.cpp \
	ui/searchwidget.cpp

HEADERS  += \
	contrib/fragments.h \
	contrib/mpfr-get_q.h \
	contrib/mpfr-impl.h \
	contrib/Copy_polyhedron_to.h \
	contrib/qtcompat.h \
	abstractsettings.h \
	application.h \
	assertexception.h \
	bedappearance.h \
	builtinmanager.h \
	cgalauxiliarybuilder.h \
	cgaldiscretemodifier.h \
	cgalgroupmodifier.h \
	cgalsanitizer.h \
	cgaltrace.h \
	codedocdeclaration.h \
	export.h \
	function/assertfunction.h \
	function/circumcenterfunction.h \
	function/fragmentsfunction.h \
	function/isundeffunction.h \
	function/ordinalfunction.h \
	geometryevaluator.h \
	git/commit.h \
	git/gitexception.h \
	git/index.h \
	git/oid.h \
	git/repository.h \
	git/repositorymanager.h \
	git/signature.h \
	git/tree.h \
	intervalexpression.h \
	intervalvalue.h \
	module/assertmodule.h \
	module/colormodule.h \
	module/cuboidmodule.h \
	module/datummodule.h \
	module/regularpolygonmodule.h \
	module/solidmodule.h \
	module/threadmodule.h \
	namedvalue.h \
	node/solidnode.h \
	operators.h \
	renderexport.h \
	tokenreader.h \
	ui/camera.h \
	ui/commitdialog.h \
	ui/mainwindow.h \
	module.h \
	syntaxtreebuilder.h \
	parameter.h \
	expression.h \
	binaryexpression.h \
	literal.h \
	valuefactory.h \
	variable.h \
	declaration.h \
	scope.h \
	modulescope.h \
	abstractsyntaxtreebuilder.h \
	dependencybuilder.h \
	instance.h \
	argument.h \
	statement.h \
	function.h \
	functionscope.h \
	compoundstatement.h \
	assignstatement.h \
	vectorexpression.h \
	ifelsestatement.h \
	forstatement.h \
	rangeexpression.h \
	unaryexpression.h \
	invocation.h \
	returnstatement.h \
	ternaryexpression.h \
	moduleimport.h \
	treevisitor.h \
	treeprinter.h \
	viewdirections.h \
	visitabletree.h \
	script.h \
	syntaxhighlighter.h \
	tokenbuilder.h \
	abstracttokenbuilder.h \
	treeevaluator.h \
	context.h \
	value.h \
	module/echomodule.h \
	numbervalue.h \
	booleanvalue.h \
	textvalue.h \
	vectorvalue.h \
	rangevalue.h \
	iterator.h \
	valueiterator.h \
	vectoriterator.h \
	rangeiterator.h \
	scriptimport.h \
	cgal.h \
	node/primitivenode.h \
	module/cubemodule.h \
	module/differencemodule.h \
	module/polyhedronmodule.h \
	module/cylindermodule.h \
	module/primitivemodule.h \
	node.h \
	node/transformationnode.h \
	cgalrenderer.h \
	renderer.h \
	point.h \
	nodevisitor.h \
	visitablenode.h \
	nodeprinter.h \
	nodeevaluator.h \
	polygon.h \
	texteditiodevice.h \
	backgroundworker.h \
	cgalbuilder.h \
	node/differencenode.h \
	node/unionnode.h \
	module/unionmodule.h \
	module/intersectionmodule.h \
	node/intersectionnode.h \
	module/translatemodule.h \
	module/symmetricdifferencemodule.h \
	node/symmetricdifferencenode.h \
	cgalprimitive.h \
	module/squaremodule.h \
	module/circlemodule.h \
	module/minkowskimodule.h \
	node/minkowskinode.h \
	module/rotatemodule.h \
	module/mirrormodule.h \
	module/scalemodule.h \
	module/spheremodule.h \
	worker.h \
	reporter.h \
	codedocparam.h \
	dxfbuilder.h \
	module/shearmodule.h \
	module/groupmodule.h \
	ui/linenumberarea.h \
	cgalexplorer.h \
	module/hullmodule.h \
	node/hullnode.h \
	module/linearextrudemodule.h \
	node/linearextrudenode.h \
	module/boundsmodule.h \
	node/boundsnode.h \
	module/subdivisionmodule.h \
	node/subdivisionnode.h \
	module/offsetmodule.h \
	node/offsetnode.h \
	module/glidemodule.h \
	node/glidenode.h \
	cgalpolygon.h \
	module/beziersurfacemodule.h \
	ui/preferencesdialog.h \
	preferences.h \
	cgalexport.h \
	module/prismmodule.h \
	function/sqrtfunction.h \
	function/sumfunction.h \
	function/randfunction.h \
	module/cylindersurfacemodule.h \
	module/importmodule.h \
	builtincreator.h \
	node/importnode.h \
	cgalimport.h \
	module/resizemodule.h \
	node/resizenode.h \
	module/rotateextrudemodule.h \
	node/rotateextrudenode.h \
	ui/saveitemsdialog.h \
	function/versionfunction.h \
	module/polygonmodule.h \
	function/lengthfunction.h \
	ui/printconsole.h \
	function/strfunction.h \
	project.h \
	function/sinfunction.h \
	function/cosfunction.h \
	function/tanfunction.h \
	function/absfunction.h \
	function/signfunction.h \
	function/minfunction.h \
	function/maxfunction.h \
	function/roundfunction.h \
	function/ceilfunction.h \
	function/floorfunction.h \
	function/powfunction.h \
	function/expfunction.h \
	function/asinfunction.h \
	function/acosfunction.h \
	function/atan2function.h \
	function/atanfunction.h \
	function/coshfunction.h \
	function/sinhfunction.h \
	function/tanhfunction.h \
	module/centermodule.h \
	ui/aboutdialog.h \
	module/pointsmodule.h \
	node/pointsnode.h \
	module/slicemodule.h \
	node/slicenode.h \
	primitive.h \
	module/conemodule.h \
	function/lnfunction.h \
	function/logfunction.h \
	module/writemodule.h \
	module/writelnmodule.h \
	callback.h \
	product.h \
	node/productnode.h \
	function/radfunction.h \
	function/degfunction.h \
	layout.h \
	module/projectionmodule.h \
	node/projectionnode.h \
	tester.h \
	cgalassert.h \
	strategy.h \
	comparer.h \
	module/multmatrixmodule.h \
	onceonly.h \
	ui/glview.h \
	fragment.h \
	cgalfragment.h \
	function/concatfunction.h \
	node/groupnode.h \
	decimal.h \
	polyhedron.h \
	module/decomposemodule.h \
	node/decomposenode.h \
	textbuilder.h \
	simpletextbuilder.h \
	interactive.h \
	ui/codeeditor.h \
	ui/console.h \
	module/alignmodule.h \
	node/alignnode.h \
	module/complementmodule.h \
	node/complementnode.h \
	module/radialsmodule.h \
	node/radialsnode.h \
	module/volumesmodule.h \
	node/volumesnode.h \
	cgalvolume.h \
	function/numfunction.h \
	rmath.h \
	module/triangulatemodule.h \
	node/triangulatenode.h \
	function/normfunction.h \
	complexexpression.h \
	complexvalue.h \
	function/angfunction.h \
	transformmatrix.h \
	module/materialmodule.h \
	node/materialnode.h \
	simplerenderer.h \
	module/discretemodule.h \
	node/discretenode.h \
	stringify.h \
	cachemanager.h \
	cache.h \
	cgalcache.h \
	emptycache.h \
	function/crossfunction.h \
	module/childrenmodule.h \
	node/normalsnode.h \
	module/normalsmodule.h \
	module/simplifymodule.h \
	node/simplifynode.h \
	contrib/qzipreader_p.h \
	contrib/qzipwriter_p.h \
	module/chainhullmodule.h \
	function/isnumfunction.h \
	function/isboolfunction.h \
	function/isstrfunction.h \
	function/islistfunction.h \
	function/israngefunction.h \
	function/isintfunction.h \
	function/chrfunction.h \
	textiterator.h \
	node/childrennode.h \
	function/ismat4x4function.h \
	asciidocprinter.h \
	generator.h \
	qpathtextbuilder.h \
	module/textmodule.h \
	module/boundarymodule.h \
	node/boundarynode.h \
	config.h \
	function/isvecfunction.h \
	cgalprojection.h \
	function/cbrtfunction.h \
	ui/searchwidget.h

FORMS += \
        ui/commitdialog.ui \
	ui/mainwindow.ui \
	ui/preferences.ui \
	ui/saveitemsdialog.ui \
	ui/printconsole.ui \
	ui/aboutdialog.ui \
	ui/searchwidget.ui

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
	icons.qrc
}

win32 {
	RC_FILE = rapcad.rc
}

RESOURCES += \
rapcad.qrc
