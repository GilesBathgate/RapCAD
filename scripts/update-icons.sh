#!/bin/bash

echo '<!DOCTYPE RCC><RCC version="1.0">' > src/icons.qrc
echo '<qresource prefix="icons/gnome">' >> src/icons.qrc
echo '    <file alias="index.theme">../icons/gnome/index.theme</file>' >> src/icons.qrc

sed -n 's/.*iconset theme="\(.*\)".*/\1.png/p' src/mainwindow.ui | while read img
do
  cp /usr/share/icons/gnome/22x22/*/$img icons/gnome/22x22/
  cp /usr/share/icons/gnome/16x16/*/$img icons/gnome/16x16/
  echo "Updated $img"
  echo "    <file alias=\"22x22/$img\">../icons/gnome/22x22/$img</file>" >> src/icons.qrc
  echo "    <file alias=\"16x16/$img\">../icons/gnome/16x16/$img</file>" >> src/icons.qrc
done
echo '</qresource>' >> src/icons.qrc
echo '</RCC>' >> src/icons.qrc
echo "Updated icons.qrc"
