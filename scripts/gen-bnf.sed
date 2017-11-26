#!/bin/sed -nf
/^%%$/,/^%%$/{
/^.*{.*}$/d;
/^%%$/d;
/^\t;$/d;
/^[A-Za-z_]\+$/{N; s/\n.*: \(.*\)/ ::=\n\t\1/};
s/TOK_//;
s@//@@;
s/DOCSTART /"\/\*\*" /;
s/ INC/ "++"/;
s/ DEC/ "--"/;
s/ ADDA/ "+="/;
s/ SUBA/ "-="/;
s/DOCEND/"\*\/"/;
s/DOCPARAM /"@" /;
s/ APPEND / "~=" /;
s/ CM / ".*" /;
s/ CD / ".\/" /;
s/ CP / "**" /;
s/ LE / "<=" /;
s/ EQ / "==" /;
s/ NE / "!=" /;
s/ GE / ">=" /;
s/ NS / "::" /;
s/ AND / "\&\&" /;
s/ OR / "||" /;
s/\([a-z_]\+\)/<\1>/g;
s/STRING/<string>/;
s/NUMBER/<number>/;
s/IDENTIFIER/<identifier>/;
s/DOCTEXT/<doctext>/;
s/[[:upper:]]\+/"\L&\E"/g;
p;}
