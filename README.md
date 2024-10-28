# MEN_Debugger
A Windows-only rudimentary CLI debugger using DWARF debug information.
Second personal project that I made.
All code base built from scratch has no dependencies other than some functions in the standard library and Win32 API to read/write register values.
The source code, although better structured and clean that my first project, still have a bad OS layer, confusing parsing strategies and a horrible way to do memory managment.

## Build instructions.
You could use the build.bat script present in the repository or just compile the file "WindowsLayer.c" with your favorite c compiler.

## How to use.
- "continue" -> the program runs until a breakpoint is hit.
- "stepi" -> execute just the next instruction.
- "out" -> step out of the function.
- "in" -> execute just the next source code line.
- "over" -> step over if the current line have a function call.
- "register [get (register name)] | [set (register name) (value)]" -> allow to modify the registers of the current thread. The register names correspond to the x86-64 achitecture.
- "breakpoint [set [line (file name) (line number)] | [func (function name)] ] | [list]" -> managin breakpoints. Breakpoints could be set to a specific line in a file or at the start of a function. If you want to see all breakopint, you can list them as well.
- "callstack" -> shows the callstack of the current thread.
- "locals" -> shows all the variable names in the current stack frame.
- "pe" -> print information inside the execuable file (file with extension .exe).
- "section [dwarf] | [abbrev] | [info] | [line]" -> allow to see the dwarf sections available, and more in detail information about the "abbrev", "info" and "line" dwarf sections.
- "pid" -> shows the process ID.
- "filenames" -> shows all the file names related to the program being debugged.
- "cu ranges" -> shows all CUs ranges.
- "exit" -> exists the program.
