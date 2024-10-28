@echo off

REM set Warnings=-W4
REM set PreprocessorFlags=-DMEM_EXPORT
REM set CompilerOptions=%Warnings% %PreprocessorFlags% -MDd -FC -Z7 -Oi -Od -GR- -EHa- -nologo
REM set LinkerOptions=-opt:ref -subsystem:windows,5.02

REM IF NOT EXIST ..\bin mkdir ..\bin
REM pushd ..\bin

REM cl %CompilerOptions% -LD ..\SourceCode\Memory_Managment.c -link %LinkerOptions% -OUT:mem.dll

REM popd

gcc ..\src\Memory_Managment.c -o mem.dll
