@echo off
@REM emu_dkm.bat - Wrapper to call CMAKE_CROSSCOMPILING_EMULATOR on Windows
@REM
@REM This file converts Windows-style paths to forward slashes, 
@REM such that the emu_dkm.sh script can consume them.
@REM
@REM USAGE:
@REM emu_dkm.bat LOGDIR TGTURI D:\path\to\kernel D:\path\to\prog.out [ARGS...]
@REM
@REM modification history
@REM --------------------
@REM 18oct16,mob  written

set mydir=%~dp0
set args=%*
@REM TODO This is not perfect because it converts \ to / even in the arguments
@REM TODO but at least it retains arguments with space embedded.
@REM TODO Ideally, only LOGDIR, KERNEL and PROG would have \ to / converted.
sh.exe %mydir:\=/%/emu_dkm.sh %args:\=/%
