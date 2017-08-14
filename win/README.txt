
Building the fast clock Tcl-module extension for Windows
========================================================

This extension currently supports build with Microsoft Visual Studio (>= 6.0):

o. Microsoft MSVC++ build:
--------------------------

To build the extension invoke the following command:

    nmake -f makefile.vc TCLDIR=<path>

You would need to give the <path> of the Tcl distribution where
tcl.h and other needed Tcl files are located.
Please look into the makefile.vc file for more information.

Alternatively, you can open the extension workspace and project files
(tclclockmod.dsw / tclclockmod.dsp resp. tclclockmod.sln / tclclockmod.vcproj) 
from within the MSVC++ and press the F7 key to build the extension under the
control of the MSVC IDE.

Example for typical build on x86 windows-systems with Visual Studio:

    if "%VCINSTALLDIR%" == "" call "%VS90COMNTOOLS%vsvars32.bat"
    set OPTDEFINES=-DWIN32 -D_WINDOWS -D_USE_32BIT_TIME_T
    set TCLDIR=..\..\tcl8.6
    nmake -nologo -f makefile.vc TCLDIR=%TCLDIR% OPTS=threads,thrdalloc OPTIMIZATIONS="-Ox -Ot -Oi -Gs" ADDOPTDEFINES="%OPTDEFINES% -DNDEBUG"

-EOF-
