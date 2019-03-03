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

#include "builtincreator.h"

#include "module/echomodule.h"
#include "module/cubemodule.h"
#include "module/squaremodule.h"
#include "module/cylindermodule.h"
#include "module/conemodule.h"
#include "module/cylindersurfacemodule.h"
#include "module/prismmodule.h"
#include "module/circlemodule.h"
#include "module/polyhedronmodule.h"
#include "module/polygonmodule.h"
#include "module/beziersurfacemodule.h"

#include "module/differencemodule.h"
#include "module/unionmodule.h"
#include "module/groupmodule.h"
#include "module/intersectionmodule.h"
#include "module/translatemodule.h"
#include "module/symmetricdifferencemodule.h"
#include "module/multmatrixmodule.h"
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
#include "module/boundarymodule.h"
#include "module/importmodule.h"
#include "module/resizemodule.h"
#include "module/centermodule.h"
#include "module/pointsmodule.h"
#include "module/slicemodule.h"
#include "module/projectionmodule.h"
#include "module/decomposemodule.h"
#include "module/alignmodule.h"
#include "module/complementmodule.h"
#include "module/radialsmodule.h"
#include "module/volumesmodule.h"
#include "module/triangulatemodule.h"
#include "module/materialmodule.h"
#include "module/discretemodule.h"
#include "module/childrenmodule.h"
#include "module/normalsmodule.h"
#include "module/simplifymodule.h"
#include "module/chainhullmodule.h"
#include "module/textmodule.h"

#include "function/lengthfunction.h"
#include "function/sqrtfunction.h"
#include "function/sumfunction.h"
#include "function/randfunction.h"
#include "function/versionfunction.h"
#include "function/strfunction.h"
#include "function/sinfunction.h"
#include "function/cosfunction.h"
#include "function/concatfunction.h"
#include "function/tanfunction.h"
#include "function/absfunction.h"
#include "function/signfunction.h"
#include "function/minfunction.h"
#include "function/maxfunction.h"
#include "function/roundfunction.h"
#include "function/ceilfunction.h"
#include "function/floorfunction.h"
#include "function/powfunction.h"
#include "function/expfunction.h"
#include "function/asinfunction.h"
#include "function/acosfunction.h"
#include "function/atanfunction.h"
#include "function/atan2function.h"
#include "function/coshfunction.h"
#include "function/sinhfunction.h"
#include "function/tanhfunction.h"
#include "function/lnfunction.h"
#include "function/logfunction.h"
#include "function/radfunction.h"
#include "function/degfunction.h"
#include "function/numfunction.h"
#include "function/normfunction.h"
#include "function/angfunction.h"
#include "function/crossfunction.h"
#include "function/isboolfunction.h"
#include "function/islistfunction.h"
#include "function/isnumfunction.h"
#include "function/israngefunction.h"
#include "function/isstrfunction.h"
#include "function/isintfunction.h"
#include "function/ismat4x4function.h"
#include "function/isvecfunction.h"
#include "function/chrfunction.h"
#include "function/cbrtfunction.h"

#include "module/writemodule.h"
#include "module/writelnmodule.h"

#include "treeprinter.h"

BuiltinCreator::BuiltinCreator(Reporter& r)
{
	builtins.append(new AlignModule(r));
	builtins.append(new BezierSurfaceModule(r));
	builtins.append(new BoundsModule(r));
	builtins.append(new CenterModule(r));
	builtins.append(new ChainHullModule(r));
	builtins.append(new ChildModule(r));
	builtins.append(new ChildrenModule(r));
	builtins.append(new CircleModule(r));
	builtins.append(new ComplementModule(r));
	builtins.append(new ConeModule(r));
	builtins.append(new CubeModule(r));
	builtins.append(new CylinderModule(r));
	builtins.append(new CylinderSurfaceModule(r));
	builtins.append(new DecomposeModule(r));
	builtins.append(new DifferenceModule(r));
	builtins.append(new DiscreteModule(r));
	builtins.append(new EchoModule(r));
	builtins.append(new GlideModule(r));
	builtins.append(new GroupModule(r));
	builtins.append(new HullModule(r));
	builtins.append(new ImportModule(r));
	builtins.append(new IntersectionModule(r));
	builtins.append(new LinearExtrudeModule(r));
	builtins.append(new MaterialModule(r));
	builtins.append(new MinkowskiModule(r));
	builtins.append(new MirrorModule(r));
	builtins.append(new MultMatrixModule(r));
	builtins.append(new NormalsModule(r));
	builtins.append(new OffsetModule(r));
	builtins.append(new BoundaryModule(r,true));
	builtins.append(new BoundaryModule(r,false));
	builtins.append(new PointsModule(r,true));
	builtins.append(new PointsModule(r,false));
	builtins.append(new PolygonModule(r,true));
	builtins.append(new PolygonModule(r,false));
	builtins.append(new PolyhedronModule(r));
	builtins.append(new PrismModule(r));
	builtins.append(new ProjectionModule(r));
	builtins.append(new RadialsModule(r));
	builtins.append(new ResizeModule(r));
	builtins.append(new RotateExtrudeModule(r));
	builtins.append(new RotateModule(r));
	builtins.append(new ScaleModule(r));
	builtins.append(new ShearModule(r));
	builtins.append(new SimplifyModule(r));
	builtins.append(new SliceModule(r));
	builtins.append(new SphereModule(r));
	builtins.append(new SquareModule(r));
	builtins.append(new SubDivisionModule(r));
	builtins.append(new SymmetricDifferenceModule(r));
	builtins.append(new TextModule(r));
	builtins.append(new TranslateModule(r));
	builtins.append(new TriangulateModule(r));
	builtins.append(new UnionModule(r));
	builtins.append(new VolumesModule(r));
	builtins.append(new WriteLnModule(r));
	builtins.append(new WriteModule(r));

	builtins.append(new AbsFunction());
	builtins.append(new AcosFunction());
	builtins.append(new AngFunction());
	builtins.append(new AsinFunction());
	builtins.append(new Atan2Function());
	builtins.append(new AtanFunction());
	builtins.append(new CbrtFunction());
	builtins.append(new CeilFunction());
	builtins.append(new ChrFunction());
	builtins.append(new ConcatFunction());
	builtins.append(new CosFunction());
	builtins.append(new CoshFunction());
	builtins.append(new CrossFunction());
	builtins.append(new DegFunction());
	builtins.append(new ExpFunction());
	builtins.append(new FloorFunction());
	builtins.append(new IsBoolFunction());
	builtins.append(new IsIntFunction());
	builtins.append(new IsListFunction());
	builtins.append(new IsNumFunction());
	builtins.append(new IsMat4x4Function());
	builtins.append(new IsRangeFunction());
	builtins.append(new IsStrFunction());
	builtins.append(new IsVecFunction(2));
	builtins.append(new IsVecFunction(3));
	builtins.append(new LengthFunction());
	builtins.append(new LnFunction());
	builtins.append(new LogFunction());
	builtins.append(new MaxFunction());
	builtins.append(new MinFunction());
	builtins.append(new NormFunction());
	builtins.append(new NumFunction());
	builtins.append(new PowFunction());
	builtins.append(new RadFunction());
	builtins.append(new RandFunction());
	builtins.append(new RoundFunction());
	builtins.append(new SignFunction());
	builtins.append(new SinFunction());
	builtins.append(new SinhFunction());
	builtins.append(new SqrtFunction());
	builtins.append(new StrFunction());
	builtins.append(new SumFunction());
	builtins.append(new TanFunction());
	builtins.append(new TanhFunction());
	builtins.append(new VersionFunction());
}

BuiltinCreator* BuiltinCreator::instance=nullptr;

BuiltinCreator* BuiltinCreator::getInstance(Reporter& r)
{
	if(!instance)
		instance = new BuiltinCreator(r);

	return instance;
}

BuiltinCreator::~BuiltinCreator()
{
	for(Declaration* d: builtins)
		delete d;
}

/**
  Add the builtins to a script.
*/
void BuiltinCreator::initBuiltins(Script& sc)
{
	for(Declaration* d: builtins)
		sc.addDeclaration(d);
}

/**
  To ensure that the builtins do not get deleted when the script
  is deleted we remove them from the script.
*/
void BuiltinCreator::saveBuiltins(Script& sc)
{
	for(Declaration* d: builtins)
		sc.removeDeclaration(d);
}

void BuiltinCreator::generateDocs(QTextStream& out)
{
	TreePrinter p(out);
	generateDocs(p);
}

void BuiltinCreator::generateDocs(TreeVisitor& p)
{
	for(Declaration* d: builtins)
		d->accept(p);
}

QHash<QString,Module*> BuiltinCreator::getModuleNames() const
{
	QHash<QString,Module*> names;
	for(Declaration* d: builtins) {
		Module* m=dynamic_cast<Module*>(d);
		if(m) names.insert(m->getName(),m);
	}
	return names;
}

void BuiltinCreator::cleanUp()
{
	delete instance;
}
