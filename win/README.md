
Building the fast clock Tcl-module extension for Windows
========================================================

This extension currently supports build with Microsoft Visual Studio (>= 6.0):

MINGW build (MINGW32/MINGW64 with MSYS):
----------------------------------

To build the extension using MINGW you will need TEA-tclconfig (the autoconf 
machinery for Tcl Extension Architecture (TEA)-compliant extensions), so clone 
it using git from https://github.com/tcltk/tclconfig, or with fossil from
https://core.tcl.tk/tclconfig, or if you have it already, you could also simply
copy/link it to the root-folder of this extension.

    # prepare for build (obtain TEA-config in win-folder):
    cd win
    git clone https://github.com/tcltk/tclconfig.git ./tclconfig

Then start msys-console, create and enter build-folder
(here as example used Release_MGW64 in win-folder of extension)
and invoke the following commands:

    # enter build folder:
    mkdir -p ./Release_MGW64 && cd ./Release_MGW64
    # configure:
    ../../configure --with-tcl=$(readlink -f ../../..)/tcl8.6/win/Release_MGW64 --enable-64bit
    # make library:
    make
    
If you have some newest incompatible toolchain resp. some changes in TEA-config,
required rebuild of `configure` script, you could reconfigure it before, using 
`autoconf` command in root of extension folder.

Microsoft MSVC++ build:
-----------------------

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

