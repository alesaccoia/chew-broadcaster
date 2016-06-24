#!/bin/sh

# 6/19/2019
# Alessandro Saccoia <alessandro@alsc.co>

# alex this is totally hardcoded to my machine, who uses homebrew
# copies the necessary files for the functioning of the QT web engine framework into
# the bin directory, then calls the build_app python scrpt that 

CMAKE_PREFIX_PATH=/usr/local/opt/qt5/ cmake ../
make -j 8

rm -rf "Chew Broadcaster.app"
rm -rf tmp

python ../install-utils/install/osx/build_app.py

cd "Chew Broadcaster.app/Contents/Resources/bin/"

# mv obs Chew

cp -R /usr/local/opt/qt5/lib/QtWebEngineCore.framework/Versions/5/Resources .

# The original QtWebEngineProcess info plist will change the name of the executable
# In the menu bar... so let's copy the one I saved
rm -rf ./Resources/Info.plist
cp ../../../../../cmake/osxbundle/QtWebEngineProcess.plist ./Resources/Info.plist

cp -R /usr/local/opt/qt5/lib/QtWebEngineCore.framework/Versions/5/Helpers .

echo "Changing the rpath for QtWebEngineProcess dependencies"
chmod +w ./Helpers/QtWebEngineProcess.app/Contents/MacOS/QtWebEngineProcess

install_name_tool -change /usr/local/Cellar/qt5/5.6.0/lib/QtWebEngineCore.framework/Versions/5/QtWebEngineCore @executable_path/../../../../QtWebEngineCore ./Helpers/QtWebEngineProcess.app/Contents/MacOS/QtWebEngineProcess
install_name_tool -change /usr/local/Cellar/qt5/5.6.0/lib/QtQuick.framework/Versions/5/QtQuick @executable_path/../../../../QtQuick ./Helpers/QtWebEngineProcess.app/Contents/MacOS/QtWebEngineProcess
install_name_tool -change /usr/local/Cellar/qt5/5.6.0/lib/QtGui.framework/Versions/5/QtGui @executable_path/../../../../QtGui ./Helpers/QtWebEngineProcess.app/Contents/MacOS/QtWebEngineProcess
install_name_tool -change /usr/local/Cellar/qt5/5.6.0/lib/QtQml.framework/Versions/5/QtQml @executable_path/../../../../QtQml ./Helpers/QtWebEngineProcess.app/Contents/MacOS/QtWebEngineProcess
install_name_tool -change /usr/local/Cellar/qt5/5.6.0/lib/QtNetwork.framework/Versions/5/QtNetwork @executable_path/../../../../QtNetwork ./Helpers/QtWebEngineProcess.app/Contents/MacOS/QtWebEngineProcess
install_name_tool -change /usr/local/Cellar/qt5/5.6.0/lib/QtWebChannel.framework/Versions/5/QtWebChannel @executable_path/../../../../QtWebChannel ./Helpers/QtWebEngineProcess.app/Contents/MacOS/QtWebEngineProcess
install_name_tool -change /usr/local/Cellar/qt5/5.6.0/lib/QtCore.framework/Versions/5/QtCore @executable_path/../../../../QtCore ./Helpers/QtWebEngineProcess.app/Contents/MacOS/QtWebEngineProcess
install_name_tool -change /usr/local/Cellar/qt5/5.6.0/lib/QtPositioning.framework/Versions/5/QtPositioning @executable_path/../../../../QtPositioning ./Helpers/QtWebEngineProcess.app/Contents/MacOS/QtWebEngineProcess

cd ../../../../

echo "Signing the application"
codesign --verbose --deep Chew\ Broadcaster.app -s "Developer ID Application: Alessandro Saccoia (DH5JXRTA39)"

echo "Verifying the application"
codesign --verbose --verify "Chew Broadcaster.app"

# echo "Zipping the app"
# tar czfv ChewBroadcaster-beta5.tgz Chew\ Broadcaster.app/
spctl --assess --type execute "Chew Broadcaster.app"

cp ../install-utils/install/osx/dmgspec.json .
cp ../install-utils/install/osx/chew-icon.png .

appdmg dmgspec.json "Chew Broadcaster.dmg"

echo "Process ok"