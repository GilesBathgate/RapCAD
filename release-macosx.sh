#!/bin/bash

qmake CONFIG+=official
make
macdeployqt rapcad.app -dmg
hdiutil internet-enable -yes rapcad.dmg
