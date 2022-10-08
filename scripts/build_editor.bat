@echo off

pushd ..\editor\scripts
call build.bat
popd

if %ERRORLEVEL% neq 0 (echo: Failed to build Calmar Editor. Erorr Code: %ERRORLEVEL% && exit)

echo Calmar Editor was build successfully.