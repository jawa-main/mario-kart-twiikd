@echo off

set KAMEK="include/kamek/Kamek/bin/debug/net6.0/Kamek.exe"
set KAMEK_SLN="include/kamek/Kamek/Kamek.sln"

set OUT="build/out/twiikd.o"
set BINOUT="build/bin/twiikd.bin"
set GECKO_OUT="build/gecko/twiikd.txt"
set DOLOUT="build/full/sys/main.dol"
set MW_SYMOUT="C:\Users\jasam\Documents\Dolphin Emulator\Maps\1_RMCE01.map"

set DOLIN="data/main-ntscu.dol"

set CC="C:\Program Files\Freescale\CW for Wii v1.7\PowerPC_EABI_Tools\Command_Line_Tools\mwcceppc.exe"

set K_STDLIB="include/kamek/k_stdlib"
set MAIN_C="src/main.cpp"
set ENTRYPOINT=0x802c0000
set SYM_TXT="data/symbols.txt"

set PY_JOIN_DOL_SYM="util/join_sym.py"
set PY_ELF2SYM="util/elf2sym.py"

set GAMEID="RMCE01"

set BUILD_KAMEK=TRUE

set LIBOGC_INCLUDE="C:\devkitPro\libogc\include"

echo ---------------COMPILATION START---------------

IF %BUILD_KAMEK% == TRUE (
    msbuild %KAMEK_SLN%
)

%CC% -I- -i %K_STDLIB% -i "src/" -i "src/revokart/" -i "src/twiiks/" -i "include/libfat/" -i "include/ogc" -Cpp_exceptions off -enum int -Os -use_lmw_stmw on -fp hard -rostr -sdata 0 -sdata2 0 -c -o %OUT% %MAIN_C% -W off
if %ERRORLEVEL% == 1 goto :end

python %PY_ELF2SYM% %OUT% %MW_SYMOUT%
if %ERRORLEVEL% == 1 goto :end

python %PY_JOIN_DOL_SYM% %GAMEID%
if %ERRORLEVEL% == 1 goto :end

%KAMEK% %OUT% -static=%ENTRYPOINT% -externals=%SYM_TXT% -input-dol=%DOLIN% -output-dol=%DOLOUT% --undef-sym-mask __F.*
if %ERRORLEVEL% == 1 goto :end

@REM wit copy build/full build/twiikd.wbfs --verbose --progress --overwrite --id=TWIIKD
@REM if %ERRORLEVEL% == 1 goto :end

:end
echo --------------COMPILATION END--------------