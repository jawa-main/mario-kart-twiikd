@echo off

set KAMEK="include/kamek/Kamek/bin/Debug/net6.0/Kamek.exe"
set OUT="build/out/nkw2.o"
set BINOUT="build/bin/nkw2.bin"
set GECKO_OUT="build/gecko/nkw2.txt"
set DOLOUT="build/full/sys/main.dol"
set DOLIN="data/main-ntscu.dol"
set CC="C:\Program Files (x86)\Freescale\CW for MPC55xx and MPC56xx 2.10\PowerPC_EABI_Tools\Command_Line_Tools\mwcceppc.exe"
set K_STDLIB="include/kamek/k_stdlib"
set MAIN_C="src/main.c"
set ENTRYPOINT=0x802c0000
set SYM_TXT="data/symbols.txt"

%CC% -I- -i %K_STDLIB% -i "src/" -Cpp_exceptions off -enum int -Os -use_lmw_stmw on -fp hard -rostr -sdata 0 -sdata2 0 -c -o %OUT% %MAIN_C%
if ERRORLEVEL 1 exit
%KAMEK% %OUT% -static=%ENTRYPOINT% -externals=%SYM_TXT% -input-dol=%DOLIN% -output-dol=%DOLOUT%
if ERRORLEVEL 1 exit
wit copy build/full build/nkw2.wbfs --verbose --progress --overwrite
@REM KAMEK OUT -dynamic -externals=SYM_TXT -output-kamek=1-dynamic.$KV$.bin

