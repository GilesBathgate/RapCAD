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

DEFINES += USE_CGAL

win32 {
	INCLUDEPATH += $$(CGAL_DIR)/include
	INCLUDEPATH += $$(CGAL_DIR)/auxiliary/gmp/include
	INCLUDEPATH += $$(BOOST_ROOT)
	LIBS += -L$$(CGAL_DIR)/lib
	exists( $$(CGAL_DIR)/lib/libCGAL* ) {
	        LIBS +=  -lCGAL -lCGAL_Core
	}
	LIBS += -L$$(CGAL_DIR)/auxiliary/gmp/lib -lmpfr-4 -lgmp-10
} else {
	exists( /usr/lib/x86_64-linux-gnu/libCGAL* ) {
	        LIBS += -lCGAL -lCGAL_Core
	}
	exists( /usr/lib/i386-linux-gnu/libCGAL* ) {
	        LIBS += -lCGAL -lCGAL_Core
	}
	LIBS += -lmpfr -lgmp -lgmpxx
  macx {
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
	LIBS += -lboost_thread
  }
}

