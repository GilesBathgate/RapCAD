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
#include "rmath.h"

#include <cmath>
#include <cstdlib>
#ifdef USE_CGAL
#include <mpfr.h>
#endif
#include "preferences.h"

#ifndef USE_CGAL
/* Tau is a more useful constant than PI and is defined as 2*PI
 * See http://tauday.com/ */
#define M_TAU		6.28318530717958647692
#endif

enum {
	MPFR_EXACT=0
};

static decimal r_round_prec(const decimal& a,int p)
{
#ifdef USE_CGAL
	mpfr_t m;
	to_mpfr(m,a);
	mpfr_prec_round(m,p,MPFR_RNDN);
	return to_decimal(m);
#else
	return a;//TODO
#endif
}

#ifdef USE_CGAL
decimal r_round_preference(mpfr_t& a,bool round)
{
	return r_round_preference(to_decimal(a),round);
}
#endif

decimal r_round_preference(const decimal& a,bool round)
{
	if(round) {
		auto& p=Preferences::getInstance();
		switch(p.getFunctionRounding()) {
			case Rounding::Decimal:
				return r_round(a,p.getDecimalPlaces());
			case Rounding::Base2:
				return r_round_prec(a,p.getSignificandBits());
			case Rounding::None:
				break;
		}
	}
	return a;
}

decimal r_tau(bool round)
{
#ifdef USE_CGAL
	return r_pi(round)*decimal(2.0);
#else
	return r_round_preference(M_TAU,round);
#endif
}

decimal r_pi(bool round)
{
#ifdef USE_CGAL
	mpfr_t m;
	mpfr_init(m);
	mpfr_const_pi(m,MPFR_RNDN);
	return r_round_preference(m,round);
#else
	return r_round_preference(M_PI,round);
#endif
}

decimal r_rad(const decimal& a,bool round)
{
	return a*(r_tau(round)/decimal(360.0));
}

decimal r_deg(const decimal& a,bool round)
{
	return a*decimal(360.0)/r_tau(round);
}

decimal r_pow(const decimal& a,const decimal& e,bool round)
{
#ifdef USE_CGAL
	mpfr_t m;
	mpfr_init(m);
	mpfr_t n;
	to_mpfr(n,a);
	mpfr_t o;
	to_mpfr(o,e);
	if(mpfr_pow(m,n,o,MPFR_RNDN)==MPFR_EXACT)
		round=false;
	mpfr_clear(n);
	mpfr_clear(o);
	return r_round_preference(m,round);
#else
	return pow(a,e);
#endif
}

decimal r_round(const decimal& a)
{
#ifdef USE_CGAL
	mpfr_t m;
	mpfr_init(m);
	mpfr_t n;
	to_mpfr(n,a);
	mpfr_round(m,n);
	mpfr_clear(n);
	return to_decimal(m);
#else
	return round(a);
#endif
}

decimal r_round(const decimal& a,int places)
{
#ifdef USE_CGAL
	mpfr_t m;
	mpfr_init(m);
	mpfr_t n;
	mpfr_init_set_si(n,10,MPFR_RNDN);
	mpfr_t o;
	mpfr_init_set_si(o,places,MPFR_RNDN);
	mpfr_pow(m,n,o,MPFR_RNDN);
	mpfr_clear(n);
	mpfr_clear(o);
	const decimal& f=to_decimal(m);
#else
	decimal f=pow(10.0,places);
#endif
	return r_round(a*f)/f;
}

decimal r_sin(const decimal& a,bool round)
{
#ifdef USE_CGAL
	mpfr_t m;
	mpfr_init(m);
	mpfr_t n;
	to_mpfr(n,a);
	mpfr_sin(m,n,MPFR_RNDN);
	mpfr_clear(n);
	return r_round_preference(m,round);
#else
	return r_round_preference(sin(a),round);
#endif
}

decimal r_cos(const decimal& a,bool round)
{
#ifdef USE_CGAL
	mpfr_t m;
	mpfr_init(m);
	mpfr_t n;
	to_mpfr(n,a);
	mpfr_cos(m,n,MPFR_RNDN);
	mpfr_clear(n);
	return r_round_preference(m,round);
#else
	return r_round_preference(cos(a),round);
#endif
}

decimal r_tan(const decimal& a,bool round)
{
#ifdef USE_CGAL
	mpfr_t m;
	mpfr_init(m);
	mpfr_t n;
	to_mpfr(n,a);
	mpfr_tan(m,n,MPFR_RNDN);
	mpfr_clear(n);
	return r_round_preference(m,round);
#else
	return r_round_preference(tan(a),round);
#endif
}

decimal r_sin_deg(const decimal& a,bool round)
{
	return r_sin(r_rad(a,false),round);
}

decimal r_cos_deg(const decimal& a,bool round)
{
	return r_cos(r_rad(a,false),round);
}

decimal r_tan_deg(const decimal& a,bool round)
{
	return r_tan(r_rad(a,false),round);
}

static bool r_right(const decimal& a)
{
	return r_mod(a,decimal(90.0))==decimal(0.0);
}

decimal r_right_sin(const decimal& a)
{
	if(r_right(a)) {
		return r_round(r_sin_deg(a,false));
	}
	return r_sin_deg(a);
}

decimal r_right_cos(const decimal& a)
{
	if(r_right(a)) {
		return r_round(r_cos_deg(a,false));
	}
	return r_cos_deg(a);
}

decimal r_sqrt(const decimal& a,bool round)
{
#ifdef USE_CGAL
	mpfr_t m;
	mpfr_init(m);
	mpfr_t n;
	to_mpfr(n,a);
	mpfr_sqrt(m,n,MPFR_RNDN);
	mpfr_clear(n);
	return r_round_preference(m,round);
#else
	return r_round_preference(sqrt(a),round);
#endif
}

decimal r_cbrt(const decimal& a,bool round)
{
#ifdef USE_CGAL
	mpfr_t m;
	mpfr_init(m);
	mpfr_t n;
	to_mpfr(n,a);
	mpfr_cbrt(m,n,MPFR_RNDN);
	mpfr_clear(n);
	return r_round_preference(m,round);
#else
	return r_round_preference(cbrt(a),round);
#endif
}

decimal r_mod(const decimal& a,const decimal& b)
{
#ifdef USE_CGAL
	mpfr_t m;
	mpfr_init(m);
	mpfr_t n;
	to_mpfr(n,a);
	mpfr_t o;
	to_mpfr(o,b);
	mpfr_fmod(m,n,o,MPFR_RNDN);
	mpfr_clear(n);
	mpfr_clear(o);
	return to_decimal(m);
#else
	return fmod(a,b);
#endif
}

decimal r_abs(const decimal& a)
{
#ifdef USE_CGAL
	return a>=decimal(0.0)?a:-a;
#else
	return fabs(a);
#endif
}

decimal r_acos(const decimal& a,bool round)
{
#ifdef USE_CGAL
	mpfr_t m;
	mpfr_init(m);
	mpfr_t n;
	to_mpfr(n,a);
	mpfr_acos(m,n,MPFR_RNDN);
	mpfr_clear(n);
	return r_round_preference(m,round);
#else
	return r_round_preference(acos(a),round);
#endif
}

decimal r_acos_deg(const decimal& a,bool round)
{
	return r_deg(r_acos(a,false),round);
}

decimal r_asin(const decimal& a,bool round)
{
#ifdef USE_CGAL
	mpfr_t m;
	mpfr_init(m);
	mpfr_t n;
	to_mpfr(n,a);
	mpfr_asin(m,n,MPFR_RNDN);
	mpfr_clear(n);
	return r_round_preference(m,round);
#else
	return r_round_preference(asin(a),round);
#endif
}

decimal r_asin_deg(const decimal& a,bool round)
{
	return r_deg(r_asin(a,false),round);
}

decimal r_atan2(const decimal& a,const decimal& b,bool round)
{
#ifdef USE_CGAL
	mpfr_t m;
	mpfr_init(m);
	mpfr_t n;
	to_mpfr(n,a);
	mpfr_t o;
	to_mpfr(o,b);
	mpfr_atan2(m,n,o,MPFR_RNDN);
	mpfr_clear(n);
	mpfr_clear(o);
	return r_round_preference(m,round);
#else
	return r_round_preference(atan2(a,b),round);
#endif
}

decimal r_atan2_deg(const decimal& a,const decimal& b,bool round)
{
	return r_deg(r_atan2(a,b,false),round);
}

decimal r_atan(const decimal& a,bool round)
{
#ifdef USE_CGAL
	mpfr_t m;
	mpfr_init(m);
	mpfr_t n;
	to_mpfr(n,a);
	mpfr_atan(m,n,MPFR_RNDN);
	mpfr_clear(n);
	return r_round_preference(m,round);
#else
	return r_round_preference(atan(a),round);
#endif
}

decimal r_atan_deg(const decimal& a,bool round)
{
	return r_deg(r_atan(a,false),round);
}

decimal r_cosh(const decimal& a,bool round)
{
#ifdef USE_CGAL
	mpfr_t m;
	mpfr_init(m);
	mpfr_t n;
	to_mpfr(n,a);
	mpfr_cosh(m,n,MPFR_RNDN);
	mpfr_clear(n);
	return r_round_preference(m,round);
#else
	return r_round_preference(cosh(a),round);
#endif
}

decimal r_cosh_deg(const decimal& a,bool round)
{
	return r_cosh(r_rad(a,false),round);
}

decimal r_sinh(const decimal& a,bool round)
{
#ifdef USE_CGAL
	mpfr_t m;
	mpfr_init(m);
	mpfr_t n;
	to_mpfr(n,a);
	mpfr_sinh(m,n,MPFR_RNDN);
	mpfr_clear(n);
	return r_round_preference(m,round);
#else
	return r_round_preference(sinh(a),round);
#endif
}

decimal r_sinh_deg(const decimal& a,bool round)
{
	return r_sinh(r_rad(a,false),round);
}

decimal r_tanh(const decimal& a,bool round)
{
#ifdef USE_CGAL
	mpfr_t m;
	mpfr_init(m);
	mpfr_t n;
	to_mpfr(n,a);
	mpfr_tanh(m,n,MPFR_RNDN);
	mpfr_clear(n);
	return r_round_preference(m,round);
#else
	return r_round_preference(tanh(a),round);
#endif
}

decimal r_tanh_deg(const decimal& a,bool round)
{
	return r_tanh(r_rad(a,false),round);
}

decimal r_max(const decimal& a,const decimal& b)
{
#ifdef USE_CGAL
	return std::max(a,b);
#else
	return fmax(a,b);
#endif
}

decimal r_min(const decimal& a,const decimal& b)
{
#ifdef USE_CGAL
	return std::min(a,b);
#else
	return fmin(a,b);
#endif
}

decimal r_ceil(const decimal& a)
{
#ifdef USE_CGAL
	mpfr_t m;
	mpfr_init(m);
	mpfr_t n;
	to_mpfr(n,a);
	mpfr_ceil(m,n);
	mpfr_clear(n);
	return to_decimal(m);
#else
	return ceil(a);
#endif
}

decimal r_floor(const decimal& a)
{
#ifdef USE_CGAL
	mpfr_t m;
	mpfr_init(m);
	mpfr_t n;
	to_mpfr(n,a);
	mpfr_floor(m,n);
	mpfr_clear(n);
	return to_decimal(m);
#else
	return floor(a);
#endif
}

decimal r_exp(const decimal& a,bool round)
{
#ifdef USE_CGAL
	mpfr_t m;
	mpfr_init(m);
	mpfr_t n;
	to_mpfr(n,a);
	mpfr_exp(m,n,MPFR_RNDN);
	mpfr_clear(n);
	return r_round_preference(m,round);
#else
	return r_round_preference(exp(a),round);
#endif
}

decimal r_log(const decimal& a,bool round)
{
#ifdef USE_CGAL
	mpfr_t m;
	mpfr_init(m);
	mpfr_t n;
	to_mpfr(n,a);
	mpfr_log(m,n,MPFR_RNDN);
	mpfr_clear(n);
	return r_round_preference(m,round);
#else
	return r_round_preference(log(a),round);
#endif
}

decimal r_log10(const decimal& a,bool round)
{
#ifdef USE_CGAL
	mpfr_t m;
	mpfr_init(m);
	mpfr_t n;
	to_mpfr(n,a);
	mpfr_log10(m,n,MPFR_RNDN);
	mpfr_clear(n);
	return r_round_preference(m,round);
#else
	return r_round_preference(log10(a),round);
#endif
}

int r_sign(const decimal& a)
{
	return a<decimal(0.0)?-1:a>decimal(0.0)?1:0;
}

#ifdef USE_CGAL
static gmp_randstate_t state;
#endif

void r_rand_seed(unsigned int seed)
{
#if USE_CGAL
	gmp_randinit_mt(state);
	gmp_randseed_ui(state,seed);
#else
	srand(seed);
#endif
}

#ifndef USE_CGAL
static decimal r_rand()
{
	return rand()/(decimal(RAND_MAX)+decimal(1));
}
#endif

decimal r_rand(const decimal& min,const decimal& max)
{
#ifdef USE_CGAL
	mpfr_t m;
	mpfr_init(m);
	mpfr_urandom(m,state,MPFR_RNDN);
	return r_round_preference((min>max)?to_decimal(m)*(min-max)+max:to_decimal(m)*(max-min)+min,true);
#else
	return (min>max)?r_rand()*(min-max)+max:r_rand()*(max-min)+min;
#endif
}

void r_rand_clear()
{
#ifdef USE_CGAL
	gmp_randclear(state);
#endif
}

bool r_is_int(const decimal& a)
{
#ifdef USE_CGAL
	mpfr_t n;
	to_mpfr(n,a);
	const bool r=mpfr_integer_p(n);
	mpfr_clear(n);
	return r;
#else
	return floor(a)==a;
#endif
}
