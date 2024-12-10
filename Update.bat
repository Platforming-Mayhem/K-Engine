RD /S /Q "%K-Engine%\Release\assets"
XCOPY "%K-Engine%\K-Engine\assets" "%K-Engine%\Release\assets" /I /E /Y /EXCLUDE:%K-Engine%\K-Engine\assets\ignore\excludeFromBuild.txt
pause