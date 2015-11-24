#!/bin/bash

function die {
  echo $1
  exit
}

SHARED=/c/shared/

pushd /c/rapcad

git reset --hard master \
  || die "failed on git reset"

git pull \
  || die "failed on git pull"

version=$(cat VERSION)
echo Building RapCAD v$version

git reset --hard master \
  || die "failed on git reset"

git clean -df \
  || die "failed on git clean"

qmake CONFIG+=official \
  || die "qmake failed."

mingw32-make -f Makefile.Release \
  || die "make failed."

mingw32-make clean \
  || die "make clean failed."

mingw32-make user_guide.html \
  || die "make userguide failed."

cp user_guide.html release \
  || die "failed copying userguide."

CGAL="../CGAL-4.7"
GMP="$CGAL/auxiliary/gmp/lib"
cp \
  $CGAL/bin/libCGAL.dll \
  $CGAL/bin/libCGAL_Core.dll \
  $GMP/libgmp-10.dll \
  $GMP/libmpfr-4.dll \
  release \
  || die "failed copying dlls."

windeployqt \
  --no-svg \
  --no-translations \
  --no-angle \
  release/rapcad.exe \
  || die "failed running windeployqt."

rm -r release/imageformats \
  || die "failed removing imageformat plugins"

makensis installer.nsi \
  || die "failed building nsis installer."

mv rapcad_setup.exe rapcad_$version\_setup.exe \
  || die "failed renaming setup exe"

mv release rapcad-$version  \
  || die "failed making release dir"

7z a -tzip rapcad_$version.zip rapcad-$version \
  || die "failed building zip"

mv rapcad_$version\_setup.exe $SHARED \
  || die "failed moving zip"

mv rapcad_$version.zip $SHARED \
  || die "failed moving setup exe"

popd
