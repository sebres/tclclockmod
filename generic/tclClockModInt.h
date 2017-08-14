/*
 * tclClockModInt.h --
 *
 * Internal declarations of fast tcl clock module.
 *
 * Copyright (c) 2017 Serg G. Brester (aka sebres)
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 */

#ifndef _TCLCLOCKMODINT_H
#define _TCLCLOCKMODINT_H

/*
 * [MSVC] fallback to replace C++ keyword "inline" with C keyword "__inline" 
 * Otherwise depending on the VC-version, context, include-order it can cause:
 *  error C2054: expected '(' to follow 'inline'
 */
#if defined(_MSC_VER) && !defined(inline)
#   define inline   __inline
#endif


/*
 * Signal using modified tcl version (dict smartref's, etc.)
 */

/* #if !defined BUILD_tcl */
#define TCL_AVAIL_SBMOD 0
#define TCL_CLOCKMOD_SCOPE
/* else 
#define TCL_AVAIL_SBMOD 1
#define TCL_CLOCKMOD_SCOPE static
*/
/* #endif */

#endif /* _TCLCLOCKMODINT_H */
