@echo off

echo Building Calmar...

pushd ..\engine\scripts
call build.bat
popd

pushd ..\editor\scripts
call build.bat
popd

echo Built Calmar Engine and Editor sucessfully.