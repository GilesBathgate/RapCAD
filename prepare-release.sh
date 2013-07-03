#!/bin/bash

newversion=$1
if [ -z "$newversion" ]; then
    echo "New version not specified"
    exit
fi

currentversion=$(cat VERSION)

export DEBEMAIL=gilesbathgate@gmail.com
dch --release-heuristic log -v$newversion "New upstream version."

sed "s/^Version:.*$/Version:        $newversion/" -i suse/rapcad.spec

git log --oneline v$currentversion..HEAD > shortlog
echo $newversion | cat - shortlog doc/release_notes.asciidoc > result
mv result doc/release_notes.asciidoc
rm shortlog
vim doc/release_notes.asciidoc

git add debian/changelog suse/rapcad.spec doc/release_notes.asciidoc
git commit -m "Updated Release notes for upcoming version."

echo $1 > VERSION
git add VERSION
git commit -m "RapCAD $newversion"

git tag -s v$1 HEAD -m "RapCAD $newversion"

echo "Now just 'git push --tags'"
