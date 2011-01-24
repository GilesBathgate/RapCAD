#!/bin/sed -nf
/^%%$/,/^%%$/{
/^.*{.*}$/d;
/^%%$/d;
/^\t;$/d;
s/^\t| /\t/;
/^[A-Za-z_]\+$/{N; s/\n.*: \(.*\)/ ::=\n\t\1/};
s/TOK_//;
s@//@@;
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
s/[[:upper:]]\+/"\L&\E"/g;
p;}
