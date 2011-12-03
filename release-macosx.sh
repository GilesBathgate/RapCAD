#!/bin/bash

qmake CONFIG+=official
make
macdeployqt -dmg
hdiutil internet-enable -yes rapcad.dmg
