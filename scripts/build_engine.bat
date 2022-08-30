@echo off

pushd ..\engine\scripts
call build.bat
popd

if %ERRORLEVEL% neq 0 (echo: Failed to build Calmar Engine. Erorr Code: %ERRORLEVEL% && exit)

echo Calmar engine was build successfully.