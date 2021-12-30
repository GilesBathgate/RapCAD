/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2021 Giles Bathgate
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
	QString s(str);
#ifdef USE_CGAL
	int i = s.indexOf('.');
	if(i>=0) {
		s.remove(i,1);
		int p=s.length()-i;
		s.append("/1");
		s.append(QString().fill('0',p));
	}
	mpq_t d;
	mpq_init(d);
	int error=mpq_set_str(d,s.toLatin1().constData(),10);
	if(error) {
		if(ok!=nullptr)
			*ok=false;
		return 0.0;
	}
	mpq_canonicalize(d);
	if(ok!=nullptr)
		*ok=true;
#ifdef CGAL_USE_GMPXX
	return mpq_class(d);
#else
	return decimal(d);
#endif
#else
	return s.toDouble(ok);
#endif
}

QString to_string(const decimal& d)
{
	Preferences& p=Preferences::getInstance();
	NumberFormats format=p.getNumberFormat();

	if(format!=NumberFormats::Scientific && d==0.0)
		return QString('0');

	QString res;
#ifdef USE_CGAL

	char* a=nullptr;
	if(format==NumberFormats::Rational) {
		gmp_asprintf(&a,"%Qd",to_mpq(d));
	} else {
		mpf_t m;
		mpf_init2(m,p.getSignificandBits());
		mpf_set_q(m,to_mpq(d));

		if(format==NumberFormats::Scientific) {
			gmp_asprintf(&a,"%.*Fe",p.getDecimalPlaces(),m);
		} else {
			gmp_asprintf(&a,"%.Ff",m);
		}
		mpf_clear(m);
	}
	res=QString(a);
	free(a);
	return res;

#else
	res.setNum(d,'f',p.getDecimalPlaces());

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
	return (n!=0.0);
}

#ifdef USE_CGAL
void to_glcoord(const Point& pt,float& x,float& y,float& z)
{
	x=to_double(pt.x());
	y=to_double(pt.y());
	z=to_double(pt.z());
}

#ifdef CGAL_USE_GMPXX
mpq_srcptr to_mpq(const decimal& d)
{
#ifndef USE_VALGRIND
	return d.exact().get_mpq_t();
#else
	return d.get_mpq_t();
#endif
}
#else
const mpq_t& to_mpq(const decimal& d)
{
#ifndef USE_VALGRIND
	return d.exact().mpq();
#else
	return d.mpq();
#endif
}
#endif

void to_mpfr(mpfr_t& m, const decimal& d)
{
	mpfr_init(m);
	mpfr_set_q(m,to_mpq(d),MPFR_RNDN);
}

decimal parse_numberexp(const QString& s, bool* ok)
{
	int i=s.indexOf('e',0,Qt::CaseInsensitive);
	if(i<0)
		return to_decimal(s);

	QString e=s.mid(i+1).remove('+');
	decimal p=to_decimal(e,ok);
	return to_decimal(s.left(i),ok) * r_pow(decimal(10),p,p<0);
}

decimal parse_rational(const QString& s, bool* ok)
{
	int i=s.lastIndexOf('/');
	if(i<0)
		return parse_numberexp(s,ok);

	return parse_rational(s.left(i),ok)/parse_numberexp(s.mid(i+1),ok);
}

decimal get_unit(const QString& s,QString& number)
{
	if(s.endsWith("um")) {
		number=s.chopped(2);
		return decimal(1.0)/1000.0;
	}
	if(s.endsWith("mm")) {
		number=s.chopped(2);
		return decimal(1.0);
	}
	if(s.endsWith("cm")) {
		number=s.chopped(2);
		return decimal(10.0);
	}
	if(s.endsWith("m")) {
		number=s.chopped(1);
		return decimal(1000.0);
	}
	if(s.endsWith("th")) {
		number=s.chopped(2);
		return decimal(254.0)/10000.0;
	}
	if(s.endsWith("in")) {
		number=s.chopped(2);
		return decimal(254.0)/10.0;
	}
	if(s.endsWith("ft")) {
		number=s.chopped(2);
		return decimal(3048.0)/10.0;
	}
	number=s;
	return decimal(1.0);
}

decimal to_decimal(const mpfr_t& m)
{
#ifdef CGAL_USE_GMPXX
	mpq_t q;
	mpq_init(q);
	mpfr_get_q(q,m);
	return mpq_class(q);
#else
	return decimal(CGAL::Gmpfr(m));
#endif
}
#endif
