#!/bin/bash
version=`cat VERSION`
releasedir=rapcad-release-test
windir=$releasedir/windows
ppadir=$releasedir/ppa

echo Building RapCAD v$version

echo Building PPA versions
pushd ../
cp -r rapcad rapcad-$version &&
if [ ! -d "$ppadir" ]; then
  mkdir -p $ppadir
fi

echo Building Precise version
pushd rapcad-$version &&
git reset --hard v$version &&
sed "s/rapcad ($version) unstable/rapcad ($version~precise1) precise/" -i  debian/changelog &&
debuild -S &&
popd &&
dput -s rapcad-ppa rapcad_$version~precise1_source.changes &&
mv rapcad_$version~precise1* $ppadir

echo Building Oneiric version
pushd rapcad-$version &&
git reset --hard v$version &&
sed "s/rapcad ($version) unstable/rapcad ($version~oneiric1) oneiric/" -i  debian/changelog &&
debuild -S &&
popd &&
dput -s rapcad-ppa rapcad_$version~oneiric1_source.changes &&
mv rapcad_$version~oneiric1* $ppadir

echo Building Natty version
pushd rapcad-$version &&
git reset --hard v$version &&
sed "s/rapcad ($version) unstable/rapcad ($version~natty1) natty/" -i  debian/changelog &&
debuild -S &&
popd &&
dput -s rapcad-ppa rapcad_$version~natty1_source.changes
mv rapcad_$version~natty1* $ppadir

rm -rf rapcad-$version
popd

#echo Building Windows version
#vboxheadless --startvm "WindowsXP-BuildEnv" &&
#if [ ! -d "$windir" ]; then
# mkdir -p $windir
#fi
#mv rapcad_$version\_setup.exe $windir/ &&
#mv rapcad_$version.zip $windir/
