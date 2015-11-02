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
#include "preferences.h"
#include "rmath.h"
#include "point.h"

decimal to_decimal(QString s,bool* ok)
{
#if USE_CGAL
	int i = s.indexOf('.');
	if(i>=0) {
		s.remove(i,1);
		int p=s.length()-i;
		s.append("/1");
		s.append(QString().fill('0',p));
	}

	CGAL::Gmpq d;
	int error=mpq_set_str(d.mpq(),s.toLatin1().constData(),10);
	if(error) {
		if(ok!=0)
			*ok=false;
		return decimal(0);
	}

	mpq_canonicalize(d.mpq());
	if(ok!=0)
		*ok=true;
	return decimal(d);
#else
	return s.toDouble(ok);
#endif
}

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

QString to_string(const decimal& d)
{
	return to_string(d,true);
}

QString to_string(const decimal& d,const bool trim)
{
	if(d==0.0)
		return QString('0');

	Preferences* p=Preferences::getInstance();
	QString res;
#if USE_CGAL

	if(p->getRationalFormat())
		return to_rational(d);

	mpf_t m;
	mp_exp_t e;
	mpf_init2(m,mpfr_get_default_prec());
	mpf_set_q(m,d.exact().mpq());
	int sign=mpf_sgn(m);
	if(sign<0)
		mpf_neg(m,m);
	char* r=mpf_get_str(NULL,&e,10,0,m);
	mpf_clear(m);
	res=QString(r);
	free(r);
	if(e>0) {
		res=res.leftJustified(e,'0');
		if(e<res.length())
			res.insert(e,'.');
	} else {
		res=res.rightJustified(-e+1,'0');
		res.prepend("0.");
	}
	if(sign<0)
		res.prepend('-');
	return res;
#else
	res.setNum(d,'f',p->getPrecision());
#endif

	if(trim) {
		//Trim trailing zeros.
		int n=amountToChop(res);
		res.chop(n);
	}

	return res;
}

int to_integer(const decimal& n)
{
#if USE_CGAL
	return int(to_double(n));
#else
	return int(n);
#endif
}

bool to_boolean(const decimal& n)
{
	return n==decimal(0)?false:true;
}

#if USE_CGAL
void to_glcoord(const Point& pt,float& x,float& y,float& z)
{
	decimal a,b,c;
	pt.getXYZ(a,b,c);
	x=to_double(a);
	y=to_double(b);
	z=to_double(c);
}

QString to_rational(const decimal& n)
{
	char* r=mpq_get_str(NULL,10,n.exact().mpq());
	QString s(r);
	free(r);
	return s;
}

CGAL::Gmpfr to_gmpfr(const decimal& d)
{
	CGAL::Gmpfr m;
	mpfr_set_q(m.fr(),d.exact().mpq(),MPFR_RNDN);
	return m;
}
#endif
