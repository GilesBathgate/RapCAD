/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2023 Giles Bathgate
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

#include "tokenbuilder.h"
#include "decimal.h"
#include "parser_yacc.h"

static constexpr int YY_NULL=0;
extern void lexerinit(yyscan_t*,AbstractTokenBuilder*,const QString&);
extern void lexerinit(yyscan_t*,AbstractTokenBuilder*,const QFileInfo&);
extern int lexerdestroy(yyscan_t);
extern void lexerinclude(yyscan_t);
extern int lexerlex(yyscan_t);
extern char* lexerget_text(yyscan_t);
extern int lexerget_leng(yyscan_t);
extern int lexerget_lineno(yyscan_t);
extern FILE* lexerget_in(yyscan_t);
extern void lexerset_in(FILE*,yyscan_t);

TokenBuilder::TokenBuilder(Reporter& r) :
	stringcontents(nullptr),
	position(0),
	reporter(r),
	parser(nullptr),
	scanner(nullptr)
{
}

QString TokenBuilder::getToken() const
{
	return token;
}

void TokenBuilder::setParser(YYSTYPE* p)
{
	parser=p;
}

TokenBuilder::TokenBuilder(Reporter& r,const QString& s) : TokenBuilder(r)
{
	lexerinit(&scanner,this,s);
}

TokenBuilder::TokenBuilder(Reporter& r,const QFileInfo& fileinfo) : TokenBuilder(r)
{
	lexerinit(&scanner,this,fileinfo);
}

TokenBuilder::~TokenBuilder()
{
	foreach(FILE* f, openfiles)
		fclose(f);
	openfiles.clear();
	lexerdestroy(scanner);
}

int TokenBuilder::nextToken()
{
	const int len=lexerget_leng(scanner);
	position+=len;
	const int next=lexerlex(scanner);
	if(next) token=QString::fromUtf8(lexerget_text(scanner),len);
	return next;
}

int TokenBuilder::getPosition() const
{
	return position;
}

int TokenBuilder::getLineNumber() const
{
	return lexerget_lineno(scanner);
}

void TokenBuilder::buildIncludeStart()
{
}

void TokenBuilder::buildIncludeFile(const QString& str)
{
	filename = str;
}

void TokenBuilder::buildIncludePath(const QString& str)
{
	filepath = str;
}

bool TokenBuilder::openfile(QFileInfo f)
{
	const QString& fullpath=f.absoluteFilePath();
	FILE* fd=fopen(QFile::encodeName(fullpath),"r");
	if(!fd) {
		reporter.reportFileMissingError(fullpath);
		return false;
	}
	openfiles.append(fd);
	lexerset_in(fd,scanner);
	return true;
}

void TokenBuilder::buildIncludeFinish()
{
	if(filename.isEmpty())
		return;

	const QDir& currentpath=pathstack.constLast();
	if(filepath.isEmpty()) {
		pathstack.push(currentpath);
	} else {
		const QFileInfo dirinfo(currentpath,filepath);
		pathstack.push(dirinfo.dir());
		filepath.clear();
	}

	const QFileInfo fileinfo(pathstack.constLast(),filename);

	if(!fileinfo.exists())
		pathstack.pop();

	filename.clear();

	if(openfile(fileinfo))
		lexerinclude(scanner);
}

void TokenBuilder::buildUseStart()
{
}

int TokenBuilder::buildUse(const QString& str)
{
	parser->text = new QString(str);
	return USE;
}

void TokenBuilder::buildUseFinish()
{
}

void TokenBuilder::buildImportStart()
{
}

int TokenBuilder::buildImport(const QString& str)
{
	parser->text = new QString(str);
	return IMPORT;
}

void TokenBuilder::buildImportFinish()
{
}

int TokenBuilder::buildModule()
{
	return MODULE;
}

int TokenBuilder::buildFunction()
{
	return FUNCTION;
}

int TokenBuilder::buildTrue()
{
	return TRUE;
}

int TokenBuilder::buildFalse()
{
	return FALSE;
}

int TokenBuilder::buildUndef()
{
	return UNDEF;
}

int TokenBuilder::buildConst()
{
	return CONST;
}

int TokenBuilder::buildParam()
{
	return PARAM;
}

int TokenBuilder::buildIf()
{
	return IF;
}

int TokenBuilder::buildAs()
{
	return AS;
}

int TokenBuilder::buildElse()
{
	return ELSE;
}

int TokenBuilder::buildFor()
{
	return FOR;
}

int TokenBuilder::buildReturn()
{
	return RETURN;
}

int TokenBuilder::buildLessEqual()
{
	return LE;
}

int TokenBuilder::buildGreatEqual()
{
	return GE;
}

int TokenBuilder::buildEqual()
{
	return EQ;
}

int TokenBuilder::buildNotEqual()
{
	return NE;
}

int TokenBuilder::buildAnd()
{
	return AND;
}

int TokenBuilder::buildOr()
{
	return OR;
}

int TokenBuilder::buildComponentwiseMultiply()
{
	return CM;
}

int TokenBuilder::buildComponentwiseDivide()
{
	return CD;
}

int TokenBuilder::buildPlusMinus()
{
	return PM;
}

int TokenBuilder::buildIncrement()
{
	return INC;
}

int TokenBuilder::buildDecrement()
{
	return DEC;
}

int TokenBuilder::buildAddAssign()
{
	return ADDA;
}

int TokenBuilder::buildSubtractAssign()
{
	return SUBA;
}

int TokenBuilder::buildCrossProduct()
{
	return CP;
}

int TokenBuilder::buildNamespace()
{
	return NS;
}

int TokenBuilder::buildAppend()
{
	return APPEND;
}

int TokenBuilder::buildOperator(int c)
{
	return c;
}

int TokenBuilder::buildLegalChar(int c)
{
	return c;
}

int TokenBuilder::buildByteOrderMark()
{
	return BOM;
}

int TokenBuilder::buildIllegalChar(const QString&)
{
	reporter.reportLexicalError(*this,lexerget_text(scanner));
	return YY_NULL;
}

int TokenBuilder::buildNumber(const QString& str)
{
	QString number(str);
	const decimal& unit=get_unit(number);
	parser->number = new decimal(to_decimal(number)*unit);
	return NUMBER;
}

int TokenBuilder::buildNumberExp(const QString& str)
{
	QString number(str);
	const decimal& unit=get_unit(number);
	parser->number = new decimal(parse_numberexp(number)*unit);
	return NUMBER;
}

int TokenBuilder::buildRational()
{
	return UNDEF;
}

int TokenBuilder::buildRational(const QString& s)
{
	QString number(s);
	const decimal& unit=get_unit(number);
	parser->number = new decimal(parse_rational(number)*unit);
	return NUMBER;
}

int TokenBuilder::buildIdentifier(const QString& str)
{
	parser->text = new QString(str);
	return IDENTIFIER;
}

void TokenBuilder::buildStringStart()
{
	stringcontents = new QString();
}

void TokenBuilder::buildString(QChar c)
{
	stringcontents->append(c);
}

void TokenBuilder::buildString(const QString& s)
{
	stringcontents->append(s);
}

int TokenBuilder::buildStringFinish()
{
	parser->text = stringcontents;
	return STRING;
}

void TokenBuilder::buildCommentStart()
{
}

void TokenBuilder::buildComment(const QString&)
{
}

void TokenBuilder::buildCommentFinish()
{
}

int TokenBuilder::buildCodeDocStart()
{
	return DOCSTART;
}

int TokenBuilder::buildCodeDoc(const QString& s)
{
	parser->text = new QString(s.trimmed());
	return DOCTEXT;
}

void TokenBuilder::buildCodeDoc()
{
}

int TokenBuilder::buildCodeDocParam(const QString& s)
{
	parser->text = new QString(s.trimmed());
	return DOCPARAM;
}

int TokenBuilder::buildCodeDocFinish()
{
	return DOCEND;
}

void TokenBuilder::buildWhiteSpaceError()
{
	position+=lexerget_leng(scanner);
}

void TokenBuilder::buildWhiteSpace()
{
	position+=lexerget_leng(scanner);
}

void TokenBuilder::buildNewLine()
{
	position=1;
}

void TokenBuilder::buildFileStart(QFileInfo file)
{
	openfile(file);
	pathstack.push(file.absoluteDir());
}

void TokenBuilder::buildFileFinish()
{
	if(!pathstack.isEmpty())
		pathstack.pop();
}
