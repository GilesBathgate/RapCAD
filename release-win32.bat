@echo off

call C:\Qt\4.8.0\bin\qtvars.bat
set PATH=%PATH%;%ProgramFiles%\Git\bin
set PATH=%PATH%;%ProgramFiles%\NSIS

pushd c:\rapcad && ^
set /p version= < VERSION && ^
echo Building RapCAD version %version% && ^
git pull && ^
git reset --hard master && ^
git clean -df && ^
qmake CONFIG+=official ^
  >> build.log 2>> errors.log && ^
make -f Makefile.Release ^
  >> build.log 2>> errors.log && ^
make clean ^
  >> build.log 2>> errors.log && ^
copy installer.nsi release ^
  >> build.log 2>> errors.log && ^
copy ..\rapcad-dlls\* release ^
  >> build.log 2>> errors.log && ^
pushd release && ^
makensis.exe installer.nsi ^
  >> build.log 2>> errors.log ^
popd
popd
