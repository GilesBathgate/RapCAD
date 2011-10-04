@echo off

qmake CONFIG+=official
%comspec% /c make -f Makefile.Release
%comspec% /c make clean
copy installer.nsi release
copy ..\rapcad-dlls\* release
cd release
"c:\Program Files\NSIS\makensis.exe" installer.nsi