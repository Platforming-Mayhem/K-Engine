RD /S /Q "%K-Engine%\Release-x64\assets"
XCOPY "%K-Engine%\K-Engine\assets" "%K-Engine%\Release-x64\assets" /I /E /Y /EXCLUDE:%K-Engine%\K-Engine\assets\excludeFromBuild.txt
pause