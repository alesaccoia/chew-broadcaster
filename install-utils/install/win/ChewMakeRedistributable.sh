#!/bin/sh

mkdir ~/Desktop/chewRelease
cp -R bin ~/Desktop/chewRelease
cp -R data ~/Desktop/chewRelease
cp -R obs-plugins ~/Desktop/chewRelease
cp qt.conf ~/Desktop/chewRelease/bin/32bit
cp qt.conf ~/Desktop/chewRelease/bin/64bit


cd ~/Desktop/chewRelease/bin/32bit
/c/Users/Alessandro/alsc/common/cpp/Qt/5.6/msvc2013/bin/windeployqt.exe chew32.exe

cd ../64bit
/c/Users/Alessandro/alsc/common/cpp/Qt/5.6/msvc2013_64/bin/windeployqt.exe chew64.exe

cd ../../

find . -name .git* -exec rm -rf {} \;
