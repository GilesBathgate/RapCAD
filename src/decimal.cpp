/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2023 Giles Bathgate
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
#ifdef USE_CGAL
#if MPFR_VERSION < MPFR_VERSION_NUM(4,1,0)
#include <contrib/mpfr-get_q.h>
#endif
#endif

decimal to_decimal(const QString& str,bool* ok)
{
	QString s(str);
#ifdef USE_CGAL
	const auto i=s.indexOf('.');
	if(i>=0) {
		s.remove(i,1);
		const auto p=s.length()-i;
		s.append("/1");
		s.append(QString(p,'0'));
	}
	mpq_t q;
	mpq_init(q);
	const int error=mpq_set_str(q,s.toLatin1().constData(),10);
	if(error) {
		mpq_clear(q);
		if(ok!=nullptr)
			*ok=false;
		return 0.0;
	}
	mpq_canonicalize(q);
	if(ok!=nullptr)
		*ok=true;

	return to_decimal(q);
#else
	return s.toDouble(ok);
#endif
}

QString to_string(const decimal& d)
{
	auto& p=Preferences::getInstance();
	const NumberFormats format=p.getNumberFormat();

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
	return static_cast<int>(to_double(n));
#else
	return static_cast<int>(n);
#endif
}

bool to_boolean(const decimal& n)
{
	return (n!=0.0);
}

decimal parse_numberexp(const QString& s, bool* ok)
{
	const auto i=s.indexOf('e',0,Qt::CaseInsensitive);
	if(i<0)
		return to_decimal(s);

	const QString& e=s.mid(i+1).remove('+');
	const decimal& p=to_decimal(e,ok);
	return to_decimal(s.left(i),ok) * r_pow(decimal(10),p);
}

decimal parse_mixed_rational(const QString& s, bool* ok)
{
	const auto m=s.indexOf(' ');
	if(m<0)
		return parse_rational(s,ok);

	return to_decimal(s.left(m))+parse_rational(s.mid(m+1),ok);
}

decimal parse_rational(const QString& s, bool* ok)
{
	const auto i=s.lastIndexOf('/');
	if(i<0)
		return parse_numberexp(s,ok);

	return parse_rational(s.left(i),ok)/parse_numberexp(s.mid(i+1),ok);
}

decimal get_unit(QString& number)
{
	if(number.endsWith("um")) {
		number.chop(2);
		return decimal(1.0)/1000.0;
	}
	if(number.endsWith("mm")) {
		number.chop(2);
		return decimal(1.0);
	}
	if(number.endsWith("cm")) {
		number.chop(2);
		return decimal(10.0);
	}
	if(number.endsWith("m")) {
		number.chop(1);
		return decimal(1000.0);
	}
	if(number.endsWith("th")) {
		number.chop(2);
		return decimal(254.0)/10000.0;
	}
	if(number.endsWith("in")) {
		number.chop(2);
		return decimal(254.0)/10.0;
	}
	if(number.endsWith("ft")) {
		number.chop(2);
		return decimal(3048.0)/10.0;
	}
	return decimal(1.0);
}

#ifdef USE_CGAL
mpq_srcptr to_mpq(const decimal& d)
{
#ifndef USE_VALGRIND
#ifdef CGAL_USE_BOOST_MP
	return d.exact().backend().data();
#else
#ifdef CGAL_USE_GMPXX
	return d.exact().get_mpq_t();
#else
	return d.exact().mpq();
#endif
#endif
#else
	return d.mpq();
#endif
}

void to_mpfr(mpfr_t& m, const decimal& d)
{
	mpfr_init_set_q(m,to_mpq(d),MPFR_RNDN);
}

decimal to_decimal(mpfr_t& m)
{
	mpq_t q;
	mpq_init(q);
	mpfr_get_q(q,m);
	mpfr_clear(m);
	return to_decimal(q);
}

decimal to_decimal(mpq_t& q)
{
#ifdef CGAL_USE_GMPXX
	decimal r(mpq_class{q});
#else
	decimal r(q);
#endif
	mpq_clear(q);
	return r;
}
#endif
