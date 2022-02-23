#!/bin/bash
version=$(cat VERSION)
releasedir=rapcad-release
windir=$releasedir/windows
ppadir=$releasedir/ppa

echo Building RapCAD v$version
echo
echo "Enter your gpg passphrase please"
read -s pass
gpgcommand="gpg --passphrase $pass --batch --no-tty --yes"

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
	debuild -S -p"$gpgcommand" &&
	popd &&
	dput rapcad-ppa rapcad_$version~"$vname"1_source.changes &&
	mv rapcad_$version~"$vname"1* $ppadir
}

ppa_build "Impish"
ppa_build "Focal"
ppa_build "Bionic"

rm -rf rapcad-$version

echo "Complete"
