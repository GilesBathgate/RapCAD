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

win32 {
    INCLUDEPATH += $$(LIBGIT2_DIR)/include
}

macx {
    INCLUDEPATH += $$(LIBGIT2_DIR)/include
}

SOURCES += \
    src/git/commit.cpp \
    src/git/gitexception.cpp \
    src/git/index.cpp \
    src/git/oid.cpp \
    src/git/repository.cpp \
    src/git/repositorymanager.cpp \
    src/git/signature.cpp \
    src/git/tree.cpp \
    src/breadboardmodel.cpp

HEADERS +=\
    src/breadboardmodel.h \
    src/git/commit.h \
    src/git/gitexception.h \
    src/git/index.h \
    src/git/oid.h \
    src/git/repository.h \
    src/git/repositorymanager.h \
    src/git/signature.h \
    src/git/tree.h
