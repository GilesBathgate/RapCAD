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

decimal r_tau(bool round=true);
decimal r_pi(bool round=true);
decimal r_rad(decimal a,bool round=true);
decimal r_deg(decimal a,bool round=true);
decimal r_max(decimal a,decimal b);
decimal r_min(decimal a,decimal b);
decimal r_pow(decimal a,decimal e,bool round=true);
decimal r_mod(decimal a,decimal b);
decimal r_round(decimal a);
decimal r_round(decimal a, int places);
decimal r_sqrt(decimal a,bool round=true);
decimal r_sin(decimal a,bool round=true);
decimal r_cos(decimal a,bool round=true);
decimal r_tan(decimal a,bool round=true);
decimal r_sin_deg(decimal a,bool round=true);
decimal r_cos_deg(decimal a,bool round=true);
decimal r_tan_deg(decimal a,bool round=true);
decimal r_right_sin(decimal a);
decimal r_right_cos(decimal a);
decimal r_abs(decimal a);
decimal r_acos_deg(decimal a,bool round=true);
decimal r_acos(decimal a,bool round=true);
decimal r_asin_deg(decimal a,bool round=true);
decimal r_atan2_deg(decimal a,decimal b,bool round=true);
decimal r_atan_deg(decimal a,bool round=true);
decimal r_cosh_deg(decimal a,bool round=true);
decimal r_sinh_deg(decimal a,bool round=true);
decimal r_tanh_deg(decimal a,bool round=true);
decimal r_ceil(decimal a);
decimal r_floor(decimal a);
decimal r_exp(decimal a,bool round=true);
decimal r_log(decimal a,bool round=true);
decimal r_log10(decimal a,bool round=true);
decimal r_sign(decimal a);
decimal r_rand(int seed, decimal a,decimal b);
bool r_is_int(decimal a);

#endif // RMATH_H
