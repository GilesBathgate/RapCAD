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

#ifndef RMATH_H
#define RMATH_H

#include "decimal.h"

decimal r_round_preference(const decimal& a,bool round=true);
decimal r_tau(bool round=true);
decimal r_pi(bool round=true);
decimal r_rad(const decimal&,bool round=true);
decimal r_deg(const decimal&,bool round=true);
decimal r_max(const decimal&,const decimal&);
decimal r_min(const decimal&,const decimal&);
decimal r_pow(const decimal&,const decimal&,bool round=true);
decimal r_mod(const decimal&,const decimal&);
decimal r_round(const decimal&);
decimal r_round(const decimal&, int places);
decimal r_sqrt(const decimal&,bool round=true);
decimal r_cbrt(const decimal&,bool round=true);
decimal r_sin(const decimal&,bool round=true);
decimal r_cos(const decimal&,bool round=true);
decimal r_tan(const decimal&,bool round=true);
decimal r_sin_deg(const decimal&,bool round=true);
decimal r_cos_deg(const decimal&,bool round=true);
decimal r_tan_deg(const decimal&,bool round=true);
decimal r_right_sin(const decimal&);
decimal r_right_cos(const decimal&);
decimal r_abs(const decimal&);
decimal r_acos_deg(const decimal&,bool round=true);
decimal r_acos(const decimal&,bool round=true);
decimal r_asin_deg(const decimal&,bool round=true);
decimal r_atan2_deg(const decimal&,const decimal&,bool round=true);
decimal r_atan_deg(const decimal&,bool round=true);
decimal r_cosh_deg(const decimal&,bool round=true);
decimal r_sinh_deg(const decimal&,bool round=true);
decimal r_tanh_deg(const decimal&,bool round=true);
decimal r_ceil(const decimal&);
decimal r_floor(const decimal&);
decimal r_exp(const decimal&,bool round=true);
decimal r_log(const decimal&,bool round=true);
decimal r_log10(const decimal&,bool round=true);
decimal r_sign(const decimal&);
void r_rand_seed(int);
decimal r_rand(const decimal&,const decimal&);
bool r_is_int(const decimal&);

#endif // RMATH_H
