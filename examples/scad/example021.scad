/*
 *  OpenSCAD (www.openscad.org)
 *  Copyright (C) 2009-2011 Clifford Wolf <clifford@clifford.at> and
 *                          Marius Kintel <marius@kintel.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

module thing()
{
	$fa = 30;
	difference() {
		sphere(r = 25);
		cylinder(h = 62.5, r1 = 12.5, r2 = 6.25, center = true);
		rotate(90, [ 1, 0, 0 ]) cylinder(h = 62.5,
				r1 = 12.5, r2 = 6.25, center = true);
		rotate(90, [ 0, 1, 0 ]) cylinder(h = 62.5,
				r1 = 12.5, r2 = 6.25, center = true);
	}
}

module demo_proj()
{
	linear_extrude(center = true, height = 0.5) projection(cut = false) thing();
	% thing();
}

module demo_cut()
{
	for (i=[-20:5:+20]) {
		 rotate(-30, [ 1, 1, 0 ]) translate([ 0, 0, -i ])
			linear_extrude(center = true, height = 0.5) projection(cut = true)
				translate([ 0, 0, i ]) rotate(+30, [ 1, 1, 0 ]) thing();
	}
	% thing();
}

translate([ -30, 0, 0 ]) demo_proj();
translate([ +30, 0, 0 ]) demo_cut();
