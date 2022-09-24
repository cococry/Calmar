@echo off

echo Building Calmar...

pushd ..\engine\scripts
call build.bat
popd

if %ERRORLEVEL% neq 0 (echo: Failed to build Calmar Engine. Erorr Code: %ERRORLEVEL% && exit)


pushd ..\editor\scripts
call build.bat
popd

if %ERRORLEVEL% neq 0 (echo: Failed to build Calmar Editor. Erorr Code: %ERRORLEVEL% && exit)



echo Built Calmar Engine and Editor sucessfully.