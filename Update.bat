RD /S /Q "Release\assets"
XCOPY "K-Engine\assets" "Release\assets" /I /E /Y /EXCLUDE:K-Engine\assets\ignore\excludeFromBuild.txt
pause