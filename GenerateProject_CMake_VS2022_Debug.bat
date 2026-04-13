@echo off
setlocal

set "ROOT=%~dp0."
set "BUILD_DIR=%~dp0build-vs2022-clangd-debug"
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
set "VSLANG=1033"

if not exist "%VSWHERE%" (
    echo Failed to find vswhere.exe.
    goto :end
)

for /f "usebackq delims=" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -find VC\Auxiliary\Build\vcvars64.bat`) do set "VSVARS=%%i"

if not defined VSVARS (
    echo Failed to find vcvars64.bat for a Visual Studio installation with C++ tools.
    goto :end
)

if not exist "%VSVARS%" (
    echo Failed to find vcvars64.bat.
    goto :end
)

call "%VSVARS%"
if errorlevel 1 goto :end

cmake -S "%ROOT%" -B "%BUILD_DIR%" -G Ninja -DCMAKE_BUILD_TYPE=Debug
if errorlevel 1 goto :end

cmake --build "%BUILD_DIR%"

:end
pause
