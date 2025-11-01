#-------------------------------------------------------------------------
#    RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
#    Copyright (C) 2010-2023 Giles Bathgate
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

include(../common.pri)

QT       += core

TARGET = rapcad
TEMPLATE = lib
CONFIG += staticlib
INCLUDEPATH += src
DESTDIR = $$OUT_PWD

DEFINES += LIBRAPCAD_LIBRARY

include(../git.pri)
include(../cgal.pri)
include(../bison.pri)

LEXSOURCES += \
    src/lexer.l

YACCSOURCES += \
    src/parser.y

SOURCES += \
    src/function/absfunction.cpp \
    src/function/acosfunction.cpp \
    src/function/angfunction.cpp \
    src/function/asinfunction.cpp \
    src/function/assertfunction.cpp \
    src/function/atan2function.cpp \
    src/function/atanfunction.cpp \
    src/function/cbrtfunction.cpp \
    src/function/ceilfunction.cpp \
    src/function/chrfunction.cpp \
    src/function/circumcenterfunction.cpp \
    src/function/concatfunction.cpp \
    src/function/cosfunction.cpp \
    src/function/coshfunction.cpp \
    src/function/crossfunction.cpp \
    src/function/degfunction.cpp \
    src/function/expfunction.cpp \
    src/function/floorfunction.cpp \
    src/function/fragmentsfunction.cpp \
    src/function/isboolfunction.cpp \
    src/function/isintfunction.cpp \
    src/function/islistfunction.cpp \
    src/function/ismat4x4function.cpp \
    src/function/isnumfunction.cpp \
    src/function/israngefunction.cpp \
    src/function/isstrfunction.cpp \
    src/function/isundeffunction.cpp \
    src/function/isvecfunction.cpp \
    src/function/lengthfunction.cpp \
    src/function/lnfunction.cpp \
    src/function/logfunction.cpp \
    src/function/maxfunction.cpp \
    src/function/minfunction.cpp \
    src/function/normfunction.cpp \
    src/function/numfunction.cpp \
    src/function/ordinalfunction.cpp \
    src/function/powfunction.cpp \
    src/function/radfunction.cpp \
    src/function/randfunction.cpp \
    src/function/roundfunction.cpp \
    src/function/signfunction.cpp \
    src/function/sinfunction.cpp \
    src/function/sinhfunction.cpp \
    src/function/sqrtfunction.cpp \
    src/function/strfunction.cpp \
    src/function/sumfunction.cpp \
    src/function/tanfunction.cpp \
    src/function/tanhfunction.cpp \
    src/function/versionfunction.cpp \
    src/git/commit.cpp \
    src/git/gitexception.cpp \
    src/git/index.cpp \
    src/git/oid.cpp \
    src/git/repository.cpp \
    src/git/repositorymanager.cpp \
    src/git/signature.cpp \
    src/git/tree.cpp \
    src/headless.cpp \
    src/node/productnode.cpp \
    src/argument.cpp \
    src/asciidocprinter.cpp \
    src/assertexception.cpp \
    src/assignstatement.cpp \
    src/binaryexpression.cpp \
    src/booleanvalue.cpp \
    src/callback.cpp \
    src/cgalfragment.cpp \
    src/codedocparam.cpp \
    src/codedocdeclaration.cpp \
    src/complexexpression.cpp \
    src/complexvalue.cpp \
    src/compoundstatement.cpp \
    src/context.cpp \
    src/decimal.cpp \
    src/declaration.cpp \
    src/expression.cpp \
    src/forstatement.cpp \
    src/fragment.cpp \
    src/function.cpp \
    src/functionscope.cpp \
    src/ifelsestatement.cpp \
    src/instance.cpp \
    src/intervalexpression.cpp \
    src/intervalvalue.cpp \
    src/invocation.cpp \
    src/layout.cpp \
    src/literal.cpp \
    src/module.cpp \
    src/moduleimport.cpp \
    src/modulescope.cpp \
    src/namedvalue.cpp \
    src/node.cpp \
    src/numbervalue.cpp \
    src/onceonly.cpp \
    src/parameter.cpp \
    src/point.cpp \
    src/polygon.cpp \
    src/preferences.cpp \
    src/product.cpp \
    src/rangeexpression.cpp \
    src/rangeiterator.cpp \
    src/rangevalue.cpp \
    src/reporter.cpp \
    src/returnstatement.cpp \
    src/rmath.cpp \
    src/scope.cpp \
    src/script.cpp \
    src/scriptimport.cpp \
    src/syntaxtreebuilder.cpp \
    src/ternaryexpression.cpp \
    src/textiterator.cpp \
    src/textvalue.cpp \
    src/tokenbuilder.cpp \
    src/tokenreader.cpp \
    src/transformmatrix.cpp \
    src/treeprinter.cpp \
    src/unaryexpression.cpp \
    src/value.cpp \
    src/valuefactory.cpp \
    src/valueiterator.cpp \
    src/variable.cpp \
    src/vectorexpression.cpp \
    src/vectoriterator.cpp \
    src/vectorvalue.cpp

HEADERS +=\
    contrib/fragments.h \
    contrib/mpfr-get_q.h \
    contrib/mpfr-impl.h \
    contrib/qtcompat.h \
    src/function/absfunction.h \
    src/function/acosfunction.h \
    src/function/angfunction.h \
    src/function/asinfunction.h \
    src/function/assertfunction.h \
    src/function/atan2function.h \
    src/function/atanfunction.h \
    src/function/cbrtfunction.h \
    src/function/ceilfunction.h \
    src/function/chrfunction.h \
    src/function/circumcenterfunction.h \
    src/function/concatfunction.h \
    src/function/cosfunction.h \
    src/function/coshfunction.h \
    src/function/crossfunction.h \
    src/function/degfunction.h \
    src/function/expfunction.h \
    src/function/floorfunction.h \
    src/function/fragmentsfunction.h \
    src/function/isboolfunction.h \
    src/function/isintfunction.h \
    src/function/islistfunction.h \
    src/function/ismat4x4function.h \
    src/function/isnumfunction.h \
    src/function/israngefunction.h \
    src/function/isstrfunction.h \
    src/function/isundeffunction.h \
    src/function/isvecfunction.h \
    src/function/lengthfunction.h \
    src/function/lnfunction.h \
    src/function/logfunction.h \
    src/function/maxfunction.h \
    src/function/minfunction.h \
    src/function/normfunction.h \
    src/function/numfunction.h \
    src/function/ordinalfunction.h \
    src/function/powfunction.h \
    src/function/radfunction.h \
    src/function/randfunction.h \
    src/function/roundfunction.h \
    src/function/signfunction.h \
    src/function/sinfunction.h \
    src/function/sinhfunction.h \
    src/function/sqrtfunction.h \
    src/function/strfunction.h \
    src/function/sumfunction.h \
    src/function/tanfunction.h \
    src/function/tanhfunction.h \
    src/function/versionfunction.h \
    src/git/commit.h \
    src/git/gitexception.h \
    src/git/index.h \
    src/git/oid.h \
    src/git/repository.h \
    src/git/repositorymanager.h \
    src/git/signature.h \
    src/git/tree.h \
    src/headless.h \
    src/node/productnode.h \
    src/abstractsettings.h \
    src/abstractsyntaxtreebuilder.h \
    src/abstracttokenbuilder.h \
    src/argument.h \
    src/asciidocprinter.h \
    src/assertexception.h \
    src/assignstatement.h \
    src/bedappearance.h \
    src/binaryexpression.h \
    src/booleanvalue.h \
    src/callback.h \
    src/cgal.h \
    src/cgalassert.h \
    src/cgalfragment.h \
    src/cgaltrace.h \
    src/codedocdeclaration.h \
    src/codedocparam.h \
    src/complexexpression.h \
    src/complexvalue.h \
    src/compoundstatement.h \
    src/config.h \
    src/context.h \
    src/decimal.h \
    src/declaration.h \
    src/expression.h \
    src/forstatement.h \
    src/fragment.h \
    src/function.h \
    src/functionscope.h \
    src/ifelsestatement.h \
    src/instance.h \
    src/intervalexpression.h \
    src/intervalvalue.h \
    src/invocation.h \
    src/iterator.h \
    src/layout.h \
    src/literal.h \
    src/module.h \
    src/moduleimport.h \
    src/modulescope.h \
    src/namedvalue.h \
    src/node.h \
    src/nodevisitor.h \
    src/numbervalue.h \
    src/onceonly.h \
    src/operators.h \
    src/parameter.h \
    src/point.h \
    src/polygon.h \
    src/preferences.h \
    src/primitive.h \
    src/product.h \
    src/rangeexpression.h \
    src/rangeiterator.h \
    src/rangevalue.h \
    src/reporter.h \
    src/returnstatement.h \
    src/rmath.h \
    src/scope.h \
    src/script.h \
    src/scriptimport.h \
    src/statement.h \
    src/syntaxtreebuilder.h \
    src/ternaryexpression.h \
    src/textiterator.h \
    src/textvalue.h \
    src/tokenbuilder.h \
    src/tokenreader.h \
    src/transformmatrix.h \
    src/treeprinter.h \
    src/treevisitor.h \
    src/unaryexpression.h \
    src/value.h \
    src/valuefactory.h \
    src/valueiterator.h \
    src/variable.h \
    src/vectorexpression.h \
    src/vectoriterator.h \
    src/vectorvalue.h \
    src/viewdirections.h \
    src/visitablenode.h \
    src/visitabletree.h
