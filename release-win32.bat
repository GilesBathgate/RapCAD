@echo off

call c:\Qt\Qt5.1.1\5.1.1\mingw48_32\bin\qtenv2.bat
set PATH=%PATH%;c:\MinGW\msys\1.0\bin
set PATH=%PATH%;%ProgramFiles%\Git\bin
set PATH=%PATH%;%ProgramFiles%\NSIS
set hostdir=h:\rapcad
set build=%hostdir%\build.log
set error=%hostdir%\error.log

pushd c:\rapcad
git pull ^
  >> %build% 2>> %error% && ^
set /p version= < VERSION
echo Building RapCAD v%version% ^
  > %build% && ^
git reset --hard master ^
  >> %build% 2>> %error% && ^
git clean -df ^
  >> %build% 2>> %error% && ^
qmake CONFIG+=official ^
  >> %build% 2>> %error% && ^
make -f Makefile.Release ^
  >> %build% 2>> %error% && ^
make clean ^
  >> %build% 2>> %error% && ^
copy ..\rapcad-dlls\* release ^
  >> %build% 2>> %error% && ^
makensis.exe installer.nsi ^
  >> %build% 2>> %error% && ^
move rapcad_setup.exe rapcad_%version%_setup.exe ^
  >> %build% 2>> %error% && ^
move release rapcad-%version% ^
  >> %build% 2>> %error% && ^
zip -r rapcad_%version%.zip rapcad-%version% ^
  >> %build% 2>> %error% && ^
move rapcad_%version%_setup.exe %hostdir% ^
  >> %build% 2>> %error% && ^
move rapcad_%version%.zip %hostdir% ^
  >> %build% 2>> %error% && ^
popd
