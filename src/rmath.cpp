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

#include <math.h>
#include <stdlib.h>

#include "rmath.h"
#include "preferences.h"

/* Tau is a more useful constant than PI and is defined as 2*PI
 * See http://tauday.com/ */
#define M_TAU		6.28318530717958647692

static decimal roundPreference(decimal a)
{
	Preferences* p=Preferences::getInstance();
	if(p->getFunctionRounding()) {
		decimal f=r_pow(10.0,p->getPrecision()-1);
		return round(a*f)/f;
	}
	return a;
}

decimal r_tau()
{
	return roundPreference(decimal(M_TAU));
}

decimal r_pi()
{
	return roundPreference(decimal(M_PI));
}

decimal r_rad(decimal a)
{
	return a*(r_tau()/decimal(360.0));
}

decimal r_deg(decimal a)
{
	return a*decimal(360.0)/r_tau();
}

decimal r_pow(decimal a,decimal e)
{
	return pow(a,e);
}

decimal r_round(decimal a)
{
	return round(a);
}

decimal r_round(decimal a,decimal b)
{
	decimal f=pow(10.0,b);
	return r_round(a*f)/f;
}

decimal r_sin(decimal a)
{
	return roundPreference(sin(a));
}

decimal r_cos(decimal a)
{
	return roundPreference(cos(a));
}

decimal r_tan(decimal a)
{
	return roundPreference(tan(a));
}

decimal r_sin_deg(decimal a)
{
	return r_sin(r_rad(a));
}

decimal r_cos_deg(decimal a)
{
	return r_cos(r_rad(a));
}

decimal r_tan_deg(decimal a)
{
	return r_tan(r_rad(a));
}

decimal r_sqrt(decimal a)
{
	return roundPreference(sqrt(a));
}

decimal r_mod(decimal a, decimal b)
{
	return fmod(a,b);
}

decimal r_abs(decimal a)
{
	return fabs(a);
}

decimal r_acos_deg(decimal a)
{
	return roundPreference(acos(r_rad(a)));
}

decimal r_asin_deg(decimal a)
{
	return roundPreference(asin(r_rad(a)));
}

decimal r_atan2_deg(decimal a,decimal b)
{
	return roundPreference(atan2(r_rad(a),r_rad(b)));
}

decimal r_atan_deg(decimal a)
{
	return roundPreference(atan(r_rad(a)));
}

decimal r_cosh_deg(decimal a)
{
	return roundPreference(cosh(r_rad(a)));
}

decimal r_sinh_deg(decimal a)
{
	return roundPreference(sinh(r_rad(a)));
}

decimal r_tanh_deg(decimal a)
{
	return roundPreference(tanh(r_rad(a)));
}

decimal r_max(decimal a, decimal b)
{
	return fmax(a,b);
}

decimal r_min(decimal a, decimal b)
{
	return fmin(a,b);
}

decimal r_ceil(decimal a)
{
	return ceil(a);
}

decimal r_floor(decimal a)
{
	return floor(a);
}

decimal r_exp(decimal a)
{
	return roundPreference(exp(a));
}

decimal r_log(decimal a)
{
	return roundPreference(log(a));
}

decimal r_log10(decimal a)
{
	return roundPreference(log10(a));
}

decimal r_sign(decimal a)
{
	decimal zero(0.0);
	return a<zero?decimal(-1.0):a>zero?decimal(1.0):zero;
}

static decimal r_rand()
{
	return rand()/(decimal(RAND_MAX)+decimal(1));
}

decimal r_rand(int seed, decimal min, decimal max)
{
	srand(seed);
	return (min>max)?r_rand()*(min-max)+max:r_rand()*(max-min)+min;
}

