cd /d %~dp0

del AES.exe

copy AES-bakup.exe AES.exe

gcc PE.c -o PE

PE

@REM PAUSE