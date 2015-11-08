#!/bin/bash

function die {
  echo $1
  exit
}

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

QtBin="../Qt/Qt5.5.1/5.5/mingw492_32/bin"
CGALBin="../CGAL-4.7/bin"
GMP="$CGALBin/auxiliary/gmp/lib"
cp \
$CGALBin/libCGAL.dll \
$CGALBin/libCGAL_Core.dll \
$QtBin/libgcc_s_dw2-1.dll \
$QtBin/libwinpthread-1.dll \
$QtBin/libstdc++-6.dll \
$QtBin/Qt5Core.dll \
$QtBin/Qt5Gui.dll \
$QtBin/Qt5Widgets.dll \
$GMP/libgmp-10.dll \
$GMP/libmpfr-4.dll \
release \
  || die "failed copying dlls."

makensis installer.nsi \
  || die "failed building nsis installer."

mv rapcad_setup.exe rapcad_$version\_setup.exe \
  || die "failed renaming setup exe"

mv release rapcad-$version  \
  || die "failed making release dir"

7z a -tzip rapcad_$version.zip rapcad-$version \
  || die "failed building zip"

mv rapcad_$version\_setup.exe $hostdir \
  || die "failed moving zip"

mv rapcad_$version.zip $hostdir \
  || die "failed moving setup exe"

popd
