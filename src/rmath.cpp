/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2017 Giles Bathgate
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

#include <math.h>
#include <stdlib.h>
#include <CGAL/Gmpfr.h>
#include <mpfr.h>

#include "rmath.h"
#include "preferences.h"

#ifndef USE_CGAL
/* Tau is a more useful constant than PI and is defined as 2*PI
 * See http://tauday.com/ */
#define M_TAU		6.28318530717958647692
#endif

static decimal roundPreference(decimal a,bool round)
{
	if(round) {
		Preferences* p=Preferences::getInstance();
		if(p->getFunctionRounding()) {
			return r_round(a,p->getPrecision());
		}
	}
	return a;
}

decimal r_tau(bool round)
{
#ifdef USE_CGAL
	return r_pi(round)*decimal(2);
#else
	return roundPreference(M_TAU,round);
#endif
}

decimal r_pi(bool round)
{
#ifdef USE_CGAL
	CGAL::Gmpfr m;
	mpfr_const_pi(m.fr(),MPFR_RNDN);
	return roundPreference(decimal(m),round);
#else
	return roundPreference(M_PI,round);
#endif
}

decimal r_rad(decimal a,bool round)
{
	return a*(r_tau(round)/decimal(360.0));
}

decimal r_deg(decimal a,bool round)
{
	return a*decimal(360.0)/r_tau(round);
}

decimal r_pow(decimal a,decimal e,bool round)
{
#ifdef USE_CGAL
	CGAL::Gmpfr m;
	CGAL::Gmpfr n=to_gmpfr(a);
	CGAL::Gmpfr o=to_gmpfr(e);
	mpfr_pow(m.fr(),n.fr(),o.fr(),MPFR_RNDN);
	return roundPreference(decimal(m),round);
#else
	return pow(a,e);
#endif
}

decimal r_round(decimal a)
{
#ifdef USE_CGAL
	CGAL::Gmpfr m;
	CGAL::Gmpfr n=to_gmpfr(a);
	mpfr_round(m.fr(),n.fr());
	return decimal(m);
#else
	return round(a);
#endif
}

decimal r_round(decimal a,int places)
{
#ifdef USE_CGAL
	CGAL::Gmpfr m;
	CGAL::Gmpfr n=to_gmpfr(10.0);
	CGAL::Gmpfr o(places);
	mpfr_pow(m.fr(),n.fr(),o.fr(),MPFR_RNDN);
	decimal f(m);
#else
	decimal f=pow(10.0,places);
#endif
	return r_round(a*f)/f;
}

decimal r_sin(decimal a,bool round)
{
#ifdef USE_CGAL
	CGAL::Gmpfr m;
	CGAL::Gmpfr n=to_gmpfr(a);
	mpfr_sin(m.fr(),n.fr(),MPFR_RNDN);
	return roundPreference(decimal(m),round);
#else
	return roundPreference(sin(a),round);
#endif
}

decimal r_cos(decimal a, bool round)
{
#ifdef USE_CGAL
	CGAL::Gmpfr m;
	CGAL::Gmpfr n=to_gmpfr(a);
	mpfr_cos(m.fr(),n.fr(),MPFR_RNDN);
	return roundPreference(decimal(m),round);
#else
	return roundPreference(cos(a),round);
#endif
}

decimal r_tan(decimal a, bool round)
{
#ifdef USE_CGAL
	CGAL::Gmpfr m;
	CGAL::Gmpfr n=to_gmpfr(a);
	mpfr_tan(m.fr(),n.fr(),MPFR_RNDN);
	return roundPreference(decimal(m),round);
#else
	return roundPreference(tan(a),round);
#endif
}

decimal r_sin_deg(decimal a,bool round)
{
	return r_sin(r_rad(a,false),round);
}

decimal r_cos_deg(decimal a,bool round)
{
	return r_cos(r_rad(a,false),round);
}

decimal r_tan_deg(decimal a,bool round)
{
	return r_tan(r_rad(a,false),round);
}

static bool r_right(decimal a)
{
	return r_mod(a,90)==decimal(0);
}

decimal r_right_sin(decimal a)
{
	if(r_right(a)) {
		return r_round(r_sin_deg(a,false));
	}
	return r_sin_deg(a);
}

decimal r_right_cos(decimal a)
{
	if(r_right(a)) {
		return r_round(r_cos_deg(a,false));
	}
	return r_cos_deg(a);
}

decimal r_sqrt(decimal a,bool round)
{
#ifdef USE_CGAL
	CGAL::Gmpfr m;
	CGAL::Gmpfr n=to_gmpfr(a);
	mpfr_sqrt(m.fr(),n.fr(),MPFR_RNDN);
	return roundPreference(decimal(m),round);
#else
	return roundPreference(sqrt(a),round);
#endif
}

decimal r_mod(decimal a, decimal b)
{
#ifdef USE_CGAL
	CGAL::Gmpfr m;
	CGAL::Gmpfr n=to_gmpfr(a);
	CGAL::Gmpfr o=to_gmpfr(b);
	mpfr_fmod(m.fr(),n.fr(),o.fr(),MPFR_RNDN);
	return decimal(m);
#else
	return fmod(a,b);
#endif
}

decimal r_abs(decimal a)
{
#ifdef USE_CGAL
	return a>=decimal(0)?a:-a;
#else
	return fabs(a);
#endif
}

decimal r_acos(decimal a,bool round)
{
#ifdef USE_CGAL
	CGAL::Gmpfr m;
	CGAL::Gmpfr n=to_gmpfr(a);
	mpfr_acos(m.fr(),n.fr(),MPFR_RNDN);
	return roundPreference(decimal(m),round);
#else
	return roundPreference(acos(a),round);
#endif
}

decimal r_acos_deg(decimal a,bool round)
{
	return r_deg(r_acos(a,false),round);
}

decimal r_asin(decimal a,bool round)
{
#ifdef USE_CGAL
	CGAL::Gmpfr m;
	CGAL::Gmpfr n=to_gmpfr(a);
	mpfr_asin(m.fr(),n.fr(),MPFR_RNDN);
	return roundPreference(decimal(m),round);
#else
	return roundPreference(asin(a),round);
#endif
}

decimal r_asin_deg(decimal a,bool round)
{
	return r_deg(r_asin(a,false),round);
}

decimal r_atan2(decimal a,decimal b,bool round)
{
#ifdef USE_CGAL
	CGAL::Gmpfr m;
	CGAL::Gmpfr n=to_gmpfr(a);
	CGAL::Gmpfr o=to_gmpfr(b);
	mpfr_atan2(m.fr(),n.fr(),o.fr(),MPFR_RNDN);
	return roundPreference(decimal(m),round);
#else
	return roundPreference(atan2(a,b),round);
#endif
}

decimal r_atan2_deg(decimal a,decimal b,bool round)
{
	return r_deg(r_atan2(a,b,false),round);
}

decimal r_atan(decimal a,bool round)
{
#ifdef USE_CGAL
	CGAL::Gmpfr m;
	CGAL::Gmpfr n=to_gmpfr(a);
	mpfr_atan(m.fr(),n.fr(),MPFR_RNDN);
	return roundPreference(decimal(m),round);
#else
	return roundPreference(atan(a),round);
#endif
}

decimal r_atan_deg(decimal a,bool round)
{
	return r_deg(r_atan(a,false),round);
}

decimal r_cosh(decimal a,bool round)
{
#ifdef USE_CGAL
	CGAL::Gmpfr m;
	CGAL::Gmpfr n=to_gmpfr(a);
	mpfr_cosh(m.fr(),n.fr(),MPFR_RNDN);
	return roundPreference(decimal(m),round);
#else
	return roundPreference(cosh(a),round);
#endif
}

decimal r_cosh_deg(decimal a,bool round)
{
	return r_cosh(r_rad(a,false),round);
}

decimal r_sinh(decimal a,bool round)
{
#ifdef USE_CGAL
	CGAL::Gmpfr m;
	CGAL::Gmpfr n=to_gmpfr(a);
	mpfr_sinh(m.fr(),n.fr(),MPFR_RNDN);
	return roundPreference(decimal(m),round);
#else
	return roundPreference(sinh(a),round);
#endif
}

decimal r_sinh_deg(decimal a,bool round)
{
	return r_sinh(r_rad(a,false),round);
}

decimal r_tanh(decimal a,bool round)
{
#ifdef USE_CGAL
	CGAL::Gmpfr m;
	CGAL::Gmpfr n=to_gmpfr(a);
	mpfr_tanh(m.fr(),n.fr(),MPFR_RNDN);
	return roundPreference(decimal(m),round);
#else
	return roundPreference(tanh(a),round);
#endif
}

decimal r_tanh_deg(decimal a,bool round)
{
	return r_tanh(r_rad(a,false),round);
}

decimal r_max(decimal a, decimal b)
{
#ifdef USE_CGAL
	return std::max(a,b);
#else
	return fmax(a,b);
#endif
}

decimal r_min(decimal a, decimal b)
{
#ifdef USE_CGAL
	return std::min(a,b);
#else
	return fmin(a,b);
#endif
}

decimal r_ceil(decimal a)
{
#ifdef USE_CGAL
	CGAL::Gmpfr m;
	CGAL::Gmpfr n=to_gmpfr(a);
	mpfr_ceil(m.fr(),n.fr());
	return decimal(m);
#else
	return ceil(a);
#endif
}

decimal r_floor(decimal a)
{
#ifdef USE_CGAL
	CGAL::Gmpfr m;
	CGAL::Gmpfr n=to_gmpfr(a);
	mpfr_floor(m.fr(),n.fr());
	return decimal(m);
#else
	return floor(a);
#endif
}

decimal r_exp(decimal a,bool round)
{
#ifdef USE_CGAL
	CGAL::Gmpfr m;
	CGAL::Gmpfr n=to_gmpfr(a);
	mpfr_exp(m.fr(),n.fr(),MPFR_RNDN);
	return roundPreference(decimal(m),round);
#else
	return roundPreference(exp(a),round);
#endif
}

decimal r_log(decimal a,bool round)
{
#ifdef USE_CGAL
	CGAL::Gmpfr m;
	CGAL::Gmpfr n=to_gmpfr(a);
	mpfr_log(m.fr(),n.fr(),MPFR_RNDN);
	return roundPreference(decimal(m),round);
#else
	return roundPreference(log(a),round);
#endif
}

decimal r_log10(decimal a,bool round)
{
#ifdef USE_CGAL
	CGAL::Gmpfr m;
	CGAL::Gmpfr n=to_gmpfr(a);
	mpfr_log10(m.fr(),n.fr(),MPFR_RNDN);
	return roundPreference(decimal(m),round);
#else
	return roundPreference(log10(a),round);
#endif
}

decimal r_sign(decimal a)
{
	decimal zero(0.0);
	return a<zero?decimal(-1.0):a>zero?decimal(1.0):zero;
}

#ifndef USE_CGAL
static decimal r_rand()
{
	return rand()/(decimal(RAND_MAX)+decimal(1));
}
#endif

decimal r_rand(int seed, decimal min, decimal max)
{
#ifdef USE_CGAL

	gmp_randstate_t s;
	gmp_randinit_mt(s);
	mpz_t n;
	mpz_init(n);
	mpz_set_ui(n,seed);
	gmp_randseed(s,n);

	CGAL::Gmpfr m;
	mpfr_urandomb(m.fr(),s);
	return (min>max)?decimal(m)*(min-max)+max:decimal(m)*(max-min)+min;
#else
	srand(seed);
	return (min>max)?r_rand()*(min-max)+max:r_rand()*(max-min)+min;
#endif
}

bool r_is_int(decimal a)
{
#ifdef USE_CGAL
	CGAL::Gmpfr n=to_gmpfr(a);
	return mpfr_integer_p(n.fr());
#else
	return floor(a)==a;
#endif
}
