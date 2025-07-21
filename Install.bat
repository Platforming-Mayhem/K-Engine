cd /d %~dp0
cmake -B bin
pause
cmake --build bin --target Editor --config Debug
pause
echo Set oWS = WScript.CreateObject("WScript.Shell") > CreateShortcut.vbs
echo sLinkFile = "%HOMEDRIVE%%HOMEPATH%\Desktop\Editor.lnk" >> CreateShortcut.vbs
echo Set oLink = oWS.CreateShortcut(sLinkFile) >> CreateShortcut.vbs
echo oLink.TargetPath = "%cd%\\bin\\Debug\\Editor.exe" >> CreateShortcut.vbs
echo oLink.WorkingDirectory = "%cd%\\bin\\Debug" >> CreateShortcut.vbs
echo oLink.Save >> CreateShortcut.vbs
cscript CreateShortcut.vbs
del CreateShortcut.vbs