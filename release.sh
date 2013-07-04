#!/bin/bash
version=$(cat VERSION)
releasedir=rapcad-release
windir=$releasedir/windows
ppadir=$releasedir/ppa

echo Building RapCAD v$version

echo Building PPA versions
pushd ../
cp -r rapcad rapcad-$version &&
if [ ! -d "$ppadir" ]; then
  mkdir -p $ppadir
fi

ppa_build(){
	echo Building $1 version
	vname=$(echo $1 | tr "[A-Z]" "[a-z]")
	pushd rapcad-$version &&
	git reset --hard v$version &&
	sed "s/rapcad ($version) unstable/rapcad ($version~"$vname"1) $vname/" -i  debian/changelog &&
	debuild -S &&
	popd &&
	dput rapcad-ppa rapcad_$version~"$vname"1_source.changes &&
	mv rapcad_$version~"$vname"1* $ppadir
}

ppa_build "Precise"
ppa_build "Quantal"
ppa_build "Raring"

rm -rf rapcad-$version

echo Building Windows version
vboxheadless --startvm "WindowsXP-BuildEnv" &&
if [ ! -d "$windir" ]; then
 mkdir -p $windir
fi
mv rapcad_$version\_setup.exe $windir/ &&
mv rapcad_$version.zip $windir/
popd
echo "Complete"
