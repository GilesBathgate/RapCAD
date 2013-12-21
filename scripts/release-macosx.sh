#!/bin/bash

qmake CONFIG+=official
make
cp -R /opt/local/lib/Resources/qt_menu.nib rapcad.app/Contents/Resources/
macdeployqt rapcad.app -dmg
hdiutil internet-enable -yes rapcad.dmg
