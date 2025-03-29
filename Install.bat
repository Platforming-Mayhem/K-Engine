cd /d %~dp0
cmake -B bin
call "%ProgramFiles%\\Microsoft Visual Studio\\2022\\Community\\Common7\\Tools\\VsDevCmd.bat" && cd bin && msbuild Editor.sln
pause
echo Set oWS = WScript.CreateObject("WScript.Shell") > CreateShortcut.vbs
echo sLinkFile = "%HOMEDRIVE%%HOMEPATH%\Desktop\Editor.lnk" >> CreateShortcut.vbs
echo Set oLink = oWS.CreateShortcut(sLinkFile) >> CreateShortcut.vbs
echo oLink.TargetPath = "%cd%\\Debug\\Editor.exe" >> CreateShortcut.vbs
echo oLink.WorkingDirectory = "%cd%\\Debug" >> CreateShortcut.vbs
echo oLink.Save >> CreateShortcut.vbs
cscript CreateShortcut.vbs
del CreateShortcut.vbs