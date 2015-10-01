#!/bin/bash
version=$(cat VERSION)
releasedir=rapcad-release-snapshot
windir=$releasedir/windows
ppadir=$releasedir/ppa
today=$(date +%Y%m%d)
snapshot="+1SNAPSHOT$today"
read -s pass
gpgcommand="gpg --passphrase $pass --batch --no-tty --yes"

echo Building RapCAD v$version$snapshot

echo Building PPA versions
pushd ../
cp -r rapcad rapcad-$version$snapshot &&
if [ ! -d "$ppadir" ]; then
  mkdir -p $ppadir
fi

ppa_build(){
	echo Building $1 version
	vname=$(echo $1 | tr "[A-Z]" "[a-z]")
	pushd rapcad-$version$snapshot &&
	git reset --hard &&
	sed "s/rapcad ($version) unstable/rapcad ($version$snapshot~"$vname"1) $vname/" -i  debian/changelog &&
	debuild -S -p"$gpgcommand" &&
	popd &&
	dput rapcad-snapshot-ppa rapcad_$version$snapshot~"$vname"1_source.changes &&
	mv rapcad_$version$snapshot~"$vname"1* $ppadir
}

ppa_build "Vivid"
ppa_build "Trusty"

rm -rf rapcad-$version$snapshot

echo "Complete"
