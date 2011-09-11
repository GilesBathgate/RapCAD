/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2011 Giles Bathgate
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

#include "builtincreator.h"

#include "module/echomodule.h"
#include "module/cubemodule.h"
#include "module/squaremodule.h"
#include "module/cylindermodule.h"
#include "module/cylindersurfacemodule.h"
#include "module/prismmodule.h"
#include "module/circlemodule.h"
#include "module/polyhedronmodule.h"
#include "module/polygonmodule.h"
#include "module/polylinemodule.h"
#include "module/beziersurfacemodule.h"

#include "module/differencemodule.h"
#include "module/unionmodule.h"
#include "module/groupmodule.h"
#include "module/intersectionmodule.h"
#include "module/translatemodule.h"
#include "module/symmetricdifferencemodule.h"
#include "module/minkowskimodule.h"
#include "module/glidemodule.h"
#include "module/linearextrudemodule.h"
#include "module/rotateextrudemodule.h"
#include "module/hullmodule.h"
#include "module/rotatemodule.h"
#include "module/mirrormodule.h"
#include "module/scalemodule.h"
#include "module/shearmodule.h"
#include "module/spheremodule.h"
#include "module/childmodule.h"
#include "module/boundsmodule.h"
#include "module/subdivisionmodule.h"
#include "module/offsetmodule.h"
#include "module/outlinemodule.h"
#include "module/importmodule.h"
#include "module/resizemodule.h"

#include "function/lengthfunction.h"
#include "function/sqrtfunction.h"
#include "function/sumfunction.h"
#include "function/randfunction.h"
#include "function/versionfunction.h"

BuiltinCreator::BuiltinCreator(QTextStream& output)
{
	builtins.append(new EchoModule(output));
	builtins.append(new CubeModule());
	builtins.append(new SquareModule());
	builtins.append(new CylinderModule());
	builtins.append(new CylinderSurfaceModule());
	builtins.append(new PrismModule());
	builtins.append(new CircleModule());
	builtins.append(new PolyhedronModule());
	builtins.append(new PolygonModule());
	builtins.append(new PolylineModule());
	builtins.append(new BezierSurfaceModule());
	builtins.append(new DifferenceModule());
	builtins.append(new UnionModule());
	builtins.append(new GroupModule());
	builtins.append(new IntersectionModule());
	builtins.append(new TranslateModule());
	builtins.append(new ResizeModule());
	builtins.append(new SymmetricDifferenceModule());
	builtins.append(new MinkowskiModule());
	builtins.append(new GlideModule());
	builtins.append(new HullModule());
	builtins.append(new LinearExtrudeModule());
	builtins.append(new RotateExtrudeModule());
	builtins.append(new RotateModule());
	builtins.append(new MirrorModule());
	builtins.append(new ScaleModule());
	builtins.append(new ShearModule());
	builtins.append(new SphereModule());
	builtins.append(new ChildModule());
	builtins.append(new BoundsModule());
	builtins.append(new SubDivisionModule());
	builtins.append(new OffsetModule());
	builtins.append(new OutlineModule());

	builtins.append(new LengthFunction());
	builtins.append(new SqrtFunction());
	builtins.append(new SumFunction());
	builtins.append(new RandFunction());
	builtins.append(new VersionFunction());
}

BuiltinCreator* BuiltinCreator::instance=NULL;

BuiltinCreator* BuiltinCreator::getInstance(QTextStream& output)
{
	if(!instance)
		instance = new BuiltinCreator(output);

	return instance;
}

/**
  Add the builtins to a script.
*/
void BuiltinCreator::initBuiltins(Script* sc)
{
	foreach(Declaration* d,builtins)
		sc->addDeclaration(d);
}

/**
  To ensure that the builtins do not get deleted when the script
  is deleted we remove them from the script.
*/
void BuiltinCreator::saveBuiltins(Script* sc)
{
	foreach(Declaration* d,builtins)
		sc->removeDeclaration(d);
}
