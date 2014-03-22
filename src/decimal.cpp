/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2014 Giles Bathgate
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

#include "decimal.h"

static inline int amountToChop(QString s)
{
	int j=0;
	//s will always be in the form X.XX..
	//so we can cheat here
	for(int i=s.size()-1; i>=0; i--) {
		QChar c=s.at(i);
		if(c!='0') {
			if(c=='.')
				j++;
			break;
		} else {
			j++;
		}
	}
	return j;
}

QString to_string(const decimal d)
{
	return to_string(d,16,true);
}

QString to_string(const decimal d, const int precision)
{
	return to_string(d,precision,false);
}

QString to_string(const decimal d,const int precision,const bool trim)
{
	QString res;
	res.setNum(d,'f',precision);

	if(trim) {
		//Trim trailing zeros.
		int n=amountToChop(res);
		res.chop(n);
	}

	return res;
}
