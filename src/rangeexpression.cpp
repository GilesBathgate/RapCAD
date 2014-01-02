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

#include "rangeexpression.h"

RangeExpression::RangeExpression()
{
	start=NULL;
	finish=NULL;
	step=NULL;
}

RangeExpression::~RangeExpression()
{
	delete start;
	delete finish;
	delete step;
}

void RangeExpression::setStart(Expression* srt)
{
	this->start = srt;
}

Expression* RangeExpression::getStart() const
{
	return this->start;
}

void RangeExpression::setFinish(Expression* fin)
{
	this->finish = fin;
}

Expression* RangeExpression::getFinish() const
{
	return this->finish;
}

void RangeExpression::setStep(Expression* stp)
{
	this->step = stp;
}

Expression* RangeExpression::getStep() const
{
	return this->step;
}

void RangeExpression::accept(TreeVisitor& v)
{
	v.visit(this);
}
