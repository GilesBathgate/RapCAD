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
#ifdef USE_CGAL
#include "cgalfragment.h"
#include "rmath.h"

CGALFragment::CGALFragment(const Context& ctx) : Fragment(ctx)
{
}

int CGALFragment::getFragments(const CGAL::Scalar& r)
{
	int fn=fragmentNumber;
	if(fn > 0)
		return (int)(fn >= 3 ? fn : 3);

	CGAL::Scalar fe=fragmentError;
	//solve R=r/cos(pi/n) for n where R=radius and r=inradius
	if(fe > 0.0 && fe < r)
		return to_integer(r_ceil(r_pi() / r_acos((r-fe)/r,false)));

	CGAL::Scalar fs=fragmentSize;
	if(fs > 0)
		 fs=r*r_tau() / fs;

	CGAL::Scalar fa=fragmentAngle;
	if(fa > 0)
		fa=CGAL::Scalar(360.0) / fa;

	CGAL::Scalar f=std::min(fa,fs);
	return std::max(int(ceil(to_double(f))),5);
}
#endif
