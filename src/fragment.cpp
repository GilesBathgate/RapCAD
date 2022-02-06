/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2021 Giles Bathgate
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

#include "fragment.h"
#include "contrib/fragments.h"
#include "numbervalue.h"
#include "context.h"

#ifdef USE_CGAL
#include "cgalfragment.h"
#endif

Fragment::Fragment(const Context& ctx) :
	fragmentNumber(0),
	fragmentSize(2.0),
	fragmentAngle(12.0),
	fragmentError(0.0)
{
	NumberValue* fnVal=dynamic_cast<NumberValue*>(ctx.getArgumentSpecial("fn"));
	if(fnVal)
		fragmentNumber=fnVal->toInteger();
	NumberValue* fsVal=dynamic_cast<NumberValue*>(ctx.getArgumentSpecial("fs"));
	if(fsVal)
		fragmentSize=fsVal->getNumber();
	NumberValue* faVal=dynamic_cast<NumberValue*>(ctx.getArgumentSpecial("fa"));
	if(faVal)
		fragmentAngle=faVal->getNumber();
	NumberValue* feVal=dynamic_cast<NumberValue*>(ctx.getArgumentSpecial("fe"));
	if(feVal)
		fragmentError=feVal->getNumber();
}

int Fragment::getFragments(const Context& ctx,const decimal& r)
{
#ifdef USE_CGAL
	return CGALFragment(ctx).getFragments(r);
#else
	return Fragment(ctx).getFragments(r);
#endif
}

Fragment* Fragment::createFragment(const Context& ctx)
{
#ifdef USE_CGAL
	return new CGALFragment(ctx);
#else
	return new Fragment(ctx);
#endif
}

#ifdef USE_CGAL
int Fragment::getFragments(const decimal&) const
{
	throw;
}
#else
int Fragment::getFragments(const decimal& r) const
{
	return get_fragments_from_r(r,fragmentNumber,fragmentSize,fragmentAngle);
}
#endif
