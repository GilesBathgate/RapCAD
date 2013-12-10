#!/bin/bash

echo Setting up environment for Qt usage...
export PATH=$PATH:/c/Qt/Qt5.1.1/5.1.1/mingw48_32/bin
export PATH=$PATH:/c/Qt/Qt5.1.1/Tools/mingw48_32/bin
export PATH=$PATH:/c/MinGW/msys/1.0/bin
export PATH=$PATH:$PROGRAMFILES/Git/bin
export PATH=$PATH:$PROGRAMFILES/NSIS

hostdir=/h/rapcad
build=$hostdir/build.log
error=$hostdir/error.log

pushd /c/rapcad

git pull \
  > $build 2> $error

version=$(cat VERSION)

echo Building RapCAD v$version \
  >> $build

git reset --hard master \
  >> $build 2>> $error

git clean -df \
  >> $build 2>> $error

qmake CONFIG+=official \
  >> $build 2>> $error

make -f Makefile.Release \
  >> $build 2>> $error

make clean \
  >> $build 2>> $error

cp ../rapcad-dlls/* release \
  >> $build 2>> $error

makensis installer.nsi \
  >> $build 2>> $error

mv rapcad_setup.exe rapcad_$version_setup.exe \
  >> $build 2>> $error

mv release rapcad-$version  \
  >> $build 2>> $error

zip -r rapcad_$version.zip rapcad-$version \
  >> $build 2>> $error

mv rapcad_$version_setup.exe $hostdir \
  >> $build 2>> $error

mv rapcad_$version.zip $hostdir \
  >> $build 2>> $error

popd
