#!/bin/bash
version=`cat VERSION`
releasedir=rapcad-release-test
windir=$releasedir/windows

echo Building RapCAD v$version

echo Building PPA versions
pushd ../
cp -r rapcad rapcad-$version &&

echo Building oniric version
pushd rapcad-$version &&
git reset --hard v$version &&
sed "s/rapcad ($version) unstable/rapcad ($version~oneiric1) oneiric/" -i  debian/changelog &&
debuild -S &&
popd &&
dput -s rapcad-ppa rapcad_$version~oneiric1_source.changes

echo Building Natty version
pushd rapcad-$version &&
git reset --hard v$version &&
sed "s/rapcad ($version) unstable/rapcad ($version~natty1) natty/" -i  debian/changelog &&
debuild -S &&
popd &&
dput -s rapcad-ppa rapcad_$version~natty1_source.changes

popd

#echo Building Windows version
#vboxheadless --startvm "WindowsXP-BuildEnv" &&
#if [ ! -d "$windir" ]; then
# mkdir -p $windir
#fi
#mv rapcad_$version\_setup.exe $windir/ &&
#mv rapcad_$version.zip $windir/
