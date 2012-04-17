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

module example006()
{
	module edgeprofile()
	{
		render(convexity = 2) difference() {
			cube([20, 20, 150], center = true);
			translate([-10, -10, 0])
				cylinder(h = 80, r = 10, center = true);
			translate([-10, -10, +40])
				sphere(r = 10);
			translate([-10, -10, -40])
				sphere(r = 10);
		}
	}

	difference()
	{
		cube(100, center = true);
		for (rot = [ [0, 0, 0], [1, 0, 0], [0, 1, 0] ]) {
			rotate(90, rot)
				for (p = [[+1, +1, 0], [-1, +1, 90], [-1, -1, 180], [+1, -1, 270]]) {
					translate([ p[0]*50, p[1]*50, 0 ])
						rotate(p[2], [0, 0, 1])
							edgeprofile();
				}
		}
		for (i = [
			[ 0, 0, [ [0, 0] ] ],
			[ 90, 0, [ [-20, -20], [+20, +20] ] ],
			[ 180, 0, [ [-20, -25], [-20, 0], [-20, +25], [+20, -25], [+20, 0], [+20, +25] ] ],
			[ 270, 0, [ [0, 0], [-25, -25], [+25, -25], [-25, +25], [+25, +25] ] ],
			[ 0, 90, [ [-25, -25], [0, 0], [+25, +25] ] ],
			[ 0, -90, [ [-25, -25], [+25, -25], [-25, +25], [+25, +25] ] ]
		]) {
			rotate(i[0], [0, 0, 1]) rotate(i[1], [1, 0, 0]) translate([0, -50, 0])
				for (j = i[2])
					translate([j[0], 0, j[1]]) sphere(10);
		}
	}
}

example006();

