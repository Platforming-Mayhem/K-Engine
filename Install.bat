cmake -B bin
pause
"%ProgramFiles%\\Microsoft Visual Studio\\2022\\Community\\Common7\\Tools\\VsDevCmd.bat" && cd bin && msbuild Editor.sln