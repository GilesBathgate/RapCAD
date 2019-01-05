/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2019 Giles Bathgate
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

decimal to_decimal(const QString& str,bool* ok)
{
#ifdef USE_CGAL
	QString s(str);
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
		if(ok!=nullptr)
			*ok=false;
		return decimal(0);
	}

	mpq_canonicalize(d.mpq());
	if(ok!=nullptr)
		*ok=true;
	return decimal(d);
#else
	return s.toDouble(ok);
#endif
}

QString to_string(const decimal& d)
{
	if(d==0.0)
		return QString('0');

	Preferences* p=Preferences::getInstance();
	QString res;
#ifdef USE_CGAL

	if(p->getRationalFormat())
		return to_rational(d);

	mpf_t m;
	mp_exp_t e;
	mpf_init2(m,mpfr_get_default_prec());
	mpf_set_q(m,d.exact().mpq());
	int sign=mpf_sgn(m);
	if(sign<0)
		mpf_neg(m,m);
	char* r=mpf_get_str(nullptr,&e,10,0,m);
	mpf_clear(m);
	res=QString(r);
	free(r);

	QString z('0');
	if(e>0) {
		res.append(z.repeated(e-1));
		if(e<res.length())
			res.insert(e,'.');
	} else {
		res.prepend(z.repeated(-e));
		res.prepend("0.");
	}
	if(sign<0)
		res.prepend('-');
	return res;
#else
	res.setNum(d,'f',p->getPrecision());

	return res;
#endif
}


int to_integer(const decimal& n)
{
#ifdef USE_CGAL
	return int(to_double(n));
#else
	return int(n);
#endif
}

bool to_boolean(const decimal& n)
{
	return n==decimal(0)?false:true;
}

#ifdef USE_CGAL
void to_glcoord(const Point& pt,float& x,float& y,float& z)
{
	x=to_double(pt.x());
	y=to_double(pt.y());
	z=to_double(pt.z());
}

QString to_rational(const decimal& n)
{
	char* r=mpq_get_str(nullptr,10,n.exact().mpq());
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

decimal parse_numberexp(const QString& s, bool *ok)
{
	int i=s.indexOf('e',0,Qt::CaseInsensitive);
	if(i<0)
		return to_decimal(s);

	QString e=s.mid(i+1).remove('+');
	decimal p=to_decimal(e,ok);
	return to_decimal(s.left(i),ok) * r_pow(decimal(10),p,p<0);
}

decimal parse_rational(const QString& s, bool *ok)
{
	int i=s.lastIndexOf('/');
	if(i<0)
		return parse_numberexp(s,ok);

	return parse_rational(s.left(i),ok)/parse_numberexp(s.mid(i+1),ok);
}
