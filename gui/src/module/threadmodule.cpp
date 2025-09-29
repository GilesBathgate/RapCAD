#include "threadmodule.h"
#include "node/primitivenode.h"
#include "node/intersectionnode.h"
#include "module/cubemodule.h"
#include "context.h"
#include "numbervalue.h"
#include "rmath.h"

ThreadModule::ThreadModule(Reporter& r) : PrimitiveModule(r, "thread")
{
	addDescription(tr("Constructs a thread"));
	addParameter("height","num",tr("The height of the thread"));
	addParameter("diameter","num",tr("The diameter of the thread"));
	addParameter("pitch","num",tr("The pitch of the thread"));
}

Node *ThreadModule::evaluate(const Context& ctx) const
{
	auto* heightValue=getParameterArgument<NumberValue>(ctx,0);
	decimal h=1.0;
	if(heightValue)
		h=heightValue->getNumber();

	auto* dValue=getParameterArgument<NumberValue>(ctx,1);
	decimal d=2.0;
	if(dValue)
		d=dValue->getNumber();

	auto* pitchValue=getParameterArgument<NumberValue>(ctx,2);
	decimal p=1.0;
	if(pitchValue)
		p=pitchValue->getNumber();

	const auto v=r_sqrt(3.0)/2.0*p;
	const auto r0=d/2.0;
	const auto r1=r0-(5.0*v/8.0);

	const int f=Fragment::getFragments(ctx,r0);

	const auto t=to_integer(h/p);
	const auto dh=t*p; // Discrete height
	const auto z0=-p*2.0; // Shift down dy 2 turns
	const auto z1=dh+p*2.0; // Shift up dy 2 turns
	const int n=(4+t)*f; // Extra two turns at top and bottom

	const auto p0=p/8.0;
	const auto p1=(p-p0)/2.0;
	const auto p2=(p+p0)/2.0;
	const auto p3=p-p0;

	const QList<Point> s0=getSpiral(r1,f,n,z0+p0,z1+p0);
	const QList<Point> s1=getSpiral(r0,f,n,z0+p1,z1+p1);
	const QList<Point> s2=getSpiral(r0,f,n,z0+p2,z1+p2);
	const QList<Point> s3=getSpiral(r1,f,n,z0+p3,z1+p3);

	const QList<QList<Point>> spirals {s0,s1,s2,s3};

	auto* pn=new PrimitiveNode();
	Primitive* pr=pn->createPrimitive();
	pn->setChildren(ctx.getInputNodes());

	for(const auto &s: spirals)
		for(const auto& pt: s)
			pr->createVertex(pt);

	const auto c0=n*4; // After the 4 spirals
	pr->createVertex(Point(0.0,0.0,z0+p0));
	const auto c1=c0+1;
	pr->createVertex(Point(0.0,0.0,z1+p3));

	for(auto i=0;i<n-1;++i) {
		const auto j=(i+1);
		for(auto b=0;b<4;++b){
			if(b>=3&&j>=n-f)continue;

			const auto k=i+b*n;
			const auto l=j+b*n;
			const auto m=b<3?k+n:i+f;
			const auto o=b<3?l+n:j+f;

			createTriangle(pr,k,m,o);
			createTriangle(pr,k,o,l);
		}
	}

	// Bottom cap
	for(auto i=0;i<f-1;++i) {
		const auto j=(i+1);
		createTriangle(pr,c0,i,j);
	}
	for(auto i=0;i<3;++i) {
		const auto j=(i+1);
		createTriangle(pr,c0,n*j,n*i);
	}
	createTriangle(pr,f-1,f,n*3);
	createTriangle(pr,c0,f-1,n*3);

	// Top cap
	const auto tn=n*4-f;
	for(auto i=tn;i<tn+f-1;++i) {
		const auto j=(i+1);
		createTriangle(pr,c1,j,i);
	}
	for(auto i=1;i<4;++i) {
		const auto j=(i+1);
		createTriangle(pr,c1,(n*i)-1,(n*j)-1);
	}
	createTriangle(pr,tn,tn-1,(n*1)-1);
	createTriangle(pr,c1,tn,(n*1)-1);

	auto cn=new PrimitiveNode();
	auto cb=cn->createPrimitive();
	CubeModule::createCuboid<Point,decimal>(cb,-d,d,-d,d,0,h);

	auto* is=new IntersectionNode();
	is->addChild(cn);
	is->addChild(pn);

	return is;
}
