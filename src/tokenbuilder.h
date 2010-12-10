/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010  Giles Bathgate
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TOKENBUILDER_H
#define TOKENBUILDER_H

#include <QString>
#include <QChar>

class TokenBuilder
{
public:
    TokenBuilder();
    unsigned int BuildUse(QString);
    unsigned int BuildModule();
    unsigned int BuildFunction();
    unsigned int BuildTrue();
    unsigned int BuildFalse();
    unsigned int BuildUndef();
    unsigned int BuildConst();
    unsigned int BuildParam();
    unsigned int BuildIf();
    unsigned int BuildElse();
    unsigned int BuildFor();
    unsigned int BuildReturn();
    unsigned int BuildLessEqual();
    unsigned int BuildGreatEqual();
    unsigned int BuildEqual();
    unsigned int BuildNotEqual();
    unsigned int BuildAnd();
    unsigned int BuildOr();
    unsigned int BuildNumber(QString);
    unsigned int BuildIdentifier(QString);
    void BuildStringStart();
    void BuildString(QChar);
    void BuildString(QString);
    unsigned int BuildStringFinish();
    unsigned int BuildComment(QString);
private:
    QString* stringcontents;
};

#endif // TOKENBUILDER_H
