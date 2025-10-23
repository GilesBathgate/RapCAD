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
