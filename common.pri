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

VERSION = $$cat(VERSION)

# Windows and macx builds use alternative: version.txt
exists(version.txt) {
	VERSION = $$cat(version.txt)
}

CONFIG(official){
	DEFINES += RAPCAD_VERSION=$$VERSION
} else {
	CONFIG(ccache) {
		DEFINES += RAPCAD_VERSION=ccache.0.0.0
	} else {
		VERSIONS = $$split(VERSION, ".")
		MAJOR = $$member(VERSIONS, 0)
		MINOR = $$member(VERSIONS, 1)
		DEFINES += RAPCAD_VERSION=$$MAJOR"."$$MINOR".git."$$system(git log -1 --pretty=format:%h)
	}
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
}

CONFIG += c++17
