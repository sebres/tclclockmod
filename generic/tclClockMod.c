/*
 * tclClockMod.c --
 *
 * Internal implementations of fast tcl clock module.
 *
 * Copyright (c) 2017 Serg G. Brester (aka sebres)
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 */


#include "tclClockModInt.h"
#include "tcl.h"

const Tcl_ObjType* tclIntTypePtr;
const Tcl_ObjType* tclWideIntTypePtr;
const Tcl_ObjType* tclBignumTypePtr;
const Tcl_ObjType* tclListTypePtr;

const char *tclEmptyStringRep;

extern void _InitModTclIntInternals(Tcl_Interp *interp);
extern int  _InitModTclIntInterp(Tcl_Interp *interp);
extern void TclClockInit(Tcl_Interp *interp);

/*
 *----------------------------------------------------------------------
 */

void _InitModTclInternals(Tcl_Interp *interp) {

    Tcl_Obj *obj;

    /*
     * There is no other way to get some internal tcl-primitives
     * w/o this tricks, regardless whether using stubs or not
     * (linked directly).
     */

  #if 0
    obj = Tcl_NewIntObj(0);
    tclIntTypePtr = obj->typePtr;
    Tcl_DecrRefCount(obj);
    obj = Tcl_NewWideIntObj(0);
    tclWideIntTypePtr = obj->typePtr;
    Tcl_DecrRefCount(obj);   
  #endif

    tclIntTypePtr = Tcl_GetObjType("int");
    tclWideIntTypePtr = Tcl_GetObjType("wideInt");
    tclBignumTypePtr = Tcl_GetObjType("bignum");
    tclListTypePtr = Tcl_GetObjType("list");

    obj = Tcl_NewObj();
    tclEmptyStringRep = obj->bytes;
    Tcl_DecrRefCount(obj);

    if (1) {
        _InitModTclIntInternals(interp);
    }
}


/*
 *----------------------------------------------------------------------
 *
 * Tclclockmod_Init --
 *
 *  Initialize the module.
 *
 * Results:
 *  TCL_OK if the package was properly initialized.
 *
 *----------------------------------------------------------------------
 */

DLLEXPORT int
Tclclockmod_Init(
    Tcl_Interp *interp) /* The current Tcl interpreter */
{
    static int initialized = 0;

    if (!initialized) {
	if (Tcl_InitStubs(interp, "8.6", 0) == NULL) {
	    return TCL_ERROR;
	}
	_InitModTclInternals(interp);
	initialized = 1;
    }
    if (_InitModTclIntInterp(interp) != TCL_OK) {
	return TCL_ERROR;
    }

    TclClockInit(interp);

    return TCL_OK;
}
