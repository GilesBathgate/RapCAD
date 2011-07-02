#!/bin/bash

echo '<!DOCTYPE RCC><RCC version="1.0">' > icons.qrc
echo '<qresource prefix="icons/gnome">' >> icons.qrc
echo '    <file alias="index.theme">icons/gnome/index.theme</file>' >> icons.qrc

sed -n 's/.*fromTheme("\(.*\)"));.*/\1.png/p' src/mainwindow.cpp | while read img
do
cp /usr/share/icons/gnome/22x22/*/$img icons/gnome/22x22/
echo "    <file alias=\"22x22/$img\">icons/gnome/22x22/$img</file>" >> icons.qrc
done
echo '</qresource>' >> icons.qrc
echo '</RCC>' >> icons.qrc

