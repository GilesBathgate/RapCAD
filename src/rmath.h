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

#ifndef RMATH_H
#define RMATH_H

#include "decimal.h"

decimal r_tau();
decimal r_pi();
decimal r_rad(decimal a);
decimal r_deg(decimal a);
decimal r_max(decimal a, decimal b);
decimal r_min(decimal a, decimal b);
decimal r_pow(decimal a,decimal e);
decimal r_mod(decimal a,decimal b);
decimal r_round(decimal a);
decimal r_round(decimal a, decimal b);
decimal r_sqrt(decimal a);
decimal r_sin(decimal a);
decimal r_cos(decimal a);
decimal r_tan(decimal a);
decimal r_sin_deg(decimal a);
decimal r_cos_deg(decimal a);
decimal r_tan_deg(decimal a);
decimal r_abs(decimal a);
decimal r_acos_deg(decimal a);
decimal r_asin_deg(decimal a);
decimal r_atan2_deg(decimal a, decimal b);
decimal r_atan_deg(decimal a);
decimal r_cosh_deg(decimal a);
decimal r_sinh_deg(decimal a);
decimal r_tanh_deg(decimal a);
decimal r_ceil(decimal a);
decimal r_floor(decimal a);
decimal r_exp(decimal a);
decimal r_log(decimal a);
decimal r_log10(decimal a);
decimal r_sign(decimal a);
decimal r_rand(int seed, decimal a,decimal b);

template<class NT>
inline decimal inexact_sqrt(NT a)
{
	return r_sqrt(to_double(a));
}

#endif // RMATH_H
