@echo off
set "SOURCE_FILES=find_leading_ones.cpp"
set "HLS_CXX_FLAGS="
:: This batch file will compile the example design to three standard targets:
::   1) test-msvc    Compile the example design to the CPU
::                   Uses Visual Studio 2010
::   2) test-x86-64  Compile the example design to the CPU
::                   Uses the Intel HLS Compiler
::   3) test-fpga    Synthesize the example design to HDL
::                   Generates a cosimulation executable to simulate the HDL
::                   Uses the Intel HLS Compiler
::   4) clean        Remove any temporary files generated by the compiler
:: Usage: build.bat <target>
:: Example: build.bat test-x86-64

:: Only one argument expected
if not "%2"=="" goto usage

:: Accept the user's target, else default to x86-64
if not "%1"=="" (
  set "TARGET=%1"
) else (
  set "TARGET=test-x86-64"
  echo No target specified, defaulting to %TARGET%
  echo Available targets: test-x86-64, test-fpga, test-msvc, clean
)

:: Any tools installed with HLS can be found relative to the location of i++
for %%I in (i++.exe) do (
  set "HLS_INSTALL_DIR=%%~dp$PATH:I"
)
set "HLS_INSTALL_DIR=%HLS_INSTALL_DIR%.."

:: Set up the compile variables
if "%TARGET%" == "test-x86-64" (
  set "CXX=i++"
  set "CXXFLAGS=%HLS_CXX_FLAGS% -march=x86-64"
  set "LFLAGS=-o %TARGET%.exe"
) else if "%TARGET%" == "test-fpga" (
  set "CXX=i++"
  set "CXXFLAGS=%HLS_CXX_FLAGS% -march=Cyclone10GX --simulator none
  ::set "CXXFLAGS=%HLS_CXX_FLAGS% -march=Cyclone10GX
  set "LFLAGS=-o %TARGET%.exe"
) else if "%TARGET%" == "test-msvc" (
  set "CXX=cl"
  set "CXXFLAGS=/I ""%HLS_INSTALL_DIR%\include"" /nologo /EHsc /wd4068 /DWIN32 /MD"
  set "LFLAGS=/link ""/libpath:%HLS_INSTALL_DIR%\host\windows64\lib"" hls_emul.lib /out:%TARGET%.exe"
) else if "%TARGET%" == "clean" (
  IF EXIST test-msvc.exe DEL /S /F /Q test-msvc.exe > NUL
  IF EXIST test-fpga.exe DEL /S /F /Q test-fpga.exe > NUL
  IF EXIST test-fpga.prj rmdir /S /Q test-fpga.prj > NUL
  IF EXIST test-x86-64.exe DEL /S /F /Q test-x86-64.exe > NUL
  goto:eof
) else (
  goto usage
)

:: Replace "" with " in the flags
set "CXXFLAGS=%CXXFLAGS:""="%"
set "LFLAGS=%LFLAGS:""="%"

:: Kick off the compile
echo %CXX% %CXXFLAGS% %SOURCE_FILES% %LFLAGS%
%CXX% %CXXFLAGS% %SOURCE_FILES% %LFLAGS%
if not ERRORLEVEL 0 (
  echo Error: Compile failed
  exit /b 1
)
echo Run %TARGET%.exe to execute the test.

:: We're done!
goto:eof

:: Dump the usage if we get unexpected input
:usage
echo Usage: build.bat [target]
echo Targets: test-msvc, test-x86-64, test-fpga, clean
echo Example: build.bat test-x86-64
exit /b 2
