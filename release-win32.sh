#!/bin/bash

function die {
  echo $1
  exit
}

hostdir=/h/rapcad
build=$hostdir/build.log
error=$hostdir/error.log

pushd /c/rapcad

git pull \
  > $build 2> $error || die "failed on git pull"

version=$(cat VERSION) || exit

echo Building RapCAD v$version \
  >> $build || exit

git reset --hard master \
  >> $build 2>> $error || die "failed on git reset"

git clean -df \
  >> $build 2>> $error || die "failed on git clean"

qmake CONFIG+=official \
  >> $build 2>> $error || die "qmake failed."

mingw32-make -f Makefile.Release \
  >> $build 2>> $error || die "make failed."

mingw32-make clean \
  >> $build 2>> $error || die "make clean failed."

cp ../rapcad-dlls/* release \
  >> $build 2>> $error || die "failed copying dlls."

makensis installer.nsi \
  >> $build 2>> $error || die "failed building nsis installer."

mv rapcad_setup.exe rapcad_$version\_setup.exe \
  >> $build 2>> $error || die "failed renaming setup exe"

mv release rapcad-$version  \
  >> $build 2>> $error || die "failed making release dir"

7z a -tzip rapcad_$version.zip rapcad-$version \
  >> $build 2>> $error || die "failed building zip"

mv rapcad_$version\_setup.exe $hostdir \
  >> $build 2>> $error || die "failed moving zip"

mv rapcad_$version.zip $hostdir \
  >> $build 2>> $error || die "failed moving setup exe"

popd
