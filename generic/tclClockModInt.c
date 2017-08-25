/*
 * tclClockModInt.c --
 *
 * Internal implementations of fast tcl clock module.
 *
 * Copyright (c) 2017 Serg G. Brester (aka sebres)
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 */


#include "tclClockModInt.h"
#include "tclInt.h"
#include "tclCompile.h"

CompileProc *_TclCompileBasicMin0ArgCmd;
CompileProc *_TclCompileBasicMin1ArgCmd;
CompileProc *_TclCompileClockClicksCmd;
CompileProc *_TclCompileClockReadingCmd;


int TclCompileBasicMin0ArgCmd(Tcl_Interp *interp, Tcl_Parse *parsePtr,
    struct Command *cmdPtr, struct CompileEnv *compEnvPtr)
{
    return _TclCompileBasicMin0ArgCmd(interp, parsePtr, cmdPtr, compEnvPtr);
};
int TclCompileBasicMin1ArgCmd(Tcl_Interp *interp, Tcl_Parse *parsePtr,
    struct Command *cmdPtr, struct CompileEnv *compEnvPtr)
{
    return _TclCompileBasicMin1ArgCmd(interp, parsePtr, cmdPtr, compEnvPtr);
};
int TclCompileClockClicksCmd(Tcl_Interp *interp, Tcl_Parse *parsePtr,
    struct Command *cmdPtr, struct CompileEnv *compEnvPtr)
{
    return _TclCompileClockClicksCmd(interp, parsePtr, cmdPtr, compEnvPtr);
};
int TclCompileClockReadingCmd(Tcl_Interp *interp, Tcl_Parse *parsePtr,
    struct Command *cmdPtr, struct CompileEnv *compEnvPtr)
{
    return _TclCompileClockReadingCmd(interp, parsePtr, cmdPtr, compEnvPtr);
};

/*
 *----------------------------------------------------------------------
 */

/*
 * Because of missing extranals (stubs/lib) for some functions like 
 * TclpGetWideClicks, TclpGetMicroseconds, etc.
 */

Tcl_ObjCmdProc *_ClockClicksObjCmd;
Tcl_ObjCmdProc *_ClockMillisecondsObjCmd;
Tcl_ObjCmdProc *_ClockMicrosecondsObjCmd;

int ClockClicksObjCmd(ClientData clientData, Tcl_Interp *interp,
   int objc, Tcl_Obj *const *objv)
{
    return _ClockClicksObjCmd(clientData, interp, objc, objv);
}
int ClockMillisecondsObjCmd(ClientData clientData, Tcl_Interp *interp,
   int objc, Tcl_Obj *const *objv)
{
    return _ClockMillisecondsObjCmd(clientData, interp, objc, objv);
}
int ClockMicrosecondsObjCmd(ClientData clientData, Tcl_Interp *interp,
   int objc, Tcl_Obj *const *objv)
{
    return _ClockMicrosecondsObjCmd(clientData, interp, objc, objv);
}

/* Currently no external declaration for tclStringHashKeyType */

static unsigned
HashStringKey(
    Tcl_HashTable *tablePtr,	/* Hash table. */
    void *keyPtr)		/* Key from which to compute hash value. */
{
    register const char *string = keyPtr;
    register unsigned int result;
    register char c;
    if ((result = UCHAR(*string)) != 0) {
	while ((c = *++string) != 0) {
	    result += (result << 3) + UCHAR(c);
	}
    }
    return result;
}
static int
CompareStringKeys(
    void *keyPtr,		/* New key to compare. */
    Tcl_HashEntry *hPtr)	/* Existing key to compare. */
{
    register const char *p1 = (const char *) keyPtr;
    register const char *p2 = (const char *) hPtr->key.string;

    return !strcmp(p1, p2);
}
const Tcl_HashKeyType tclStringHashKeyType = {
    TCL_HASH_KEY_TYPE_VERSION,		/* version */
    0,					/* flags */
    HashStringKey,			/* hashKeyProc */
    CompareStringKeys,			/* compareKeysProc */
    NULL,				/* allocEntryProc (overriden) */
    NULL				/* freeEntryProc (overriden) */
};


/*
 *----------------------------------------------------------------------
 *
 * Tcl_DictObjSmartRef --
 *
 * Workaround to simulate dict smartref: guarantees editable dict (as single reference)
 *
 *----------------------------------------------------------------------
 */

Tcl_Obj *
Tcl_DictObjSmartRef(
    Tcl_Interp *interp,
    Tcl_Obj    *dictPtr)
{
/*    if (Tcl_IsShared(dictPtr)) { */
	dictPtr = Tcl_DuplicateObj(dictPtr);
/*    } */
    return dictPtr;
}


/*
 *----------------------------------------------------------------------
 */

/* MODULE_SCOPE */
size_t TclEnvEpoch = 0;		/* Epoch of the tcl environment
				 * (if changed with tcl-env). */

/* ARGSUSED */
static char *
EnvEpochTraceProc(
    ClientData clientData,	/* Not used. */
    Tcl_Interp *interp,		/* Interpreter whose "env" variable is being
				 * modified. */
    const char *name1,		/* Better be "env". */
    const char *name2,		/* Name of variable being modified, or NULL if
				 * whole array is being deleted (UTF-8). */
    int flags)			/* Indicates what's happening. */
{
    /*
     * Increase env-epoch if changed.
     */

    if (flags & TCL_TRACE_ARRAY) {
	TclEnvEpoch++;
	return NULL;
    }
    if (name2 == NULL) {
	return NULL;
    }
    if (flags & TCL_TRACE_WRITES) {
	TclEnvEpoch++;
    }
    else
    if (flags & TCL_TRACE_UNSETS) {
	TclEnvEpoch++;
    }
    return NULL;
}


/*
 * TclpCompileEnsemblObjCmd --
 */
int TclpCompileEnsemblObjCmd(
    ClientData unused, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    Tcl_Command token = Tcl_FindCommand(interp, "clock", NULL, TCL_GLOBAL_ONLY);
    return Tcl_SetEnsembleFlags(interp, token, ENSEMBLE_COMPILE);
}


#define InterpCommand(interp, cmdName) \
    ((Command*)Tcl_FindCommand(interp, (cmdName), NULL, TCL_GLOBAL_ONLY))

/*
 * _InitModTclIntInternals --
 */
void _InitModTclIntInternals(Tcl_Interp *interp) {

    /*
     * There is no other way to get some internal tcl-primitives
     * w/o this tricks, regardless whether using stubs or not
     * (linked directly).
     */
    _TclCompileBasicMin0ArgCmd =
	InterpCommand(interp, "::tcl::namespace::export")->compileProc;
    _TclCompileBasicMin1ArgCmd =
	InterpCommand(interp, "::tcl::dict::remove")->compileProc;
    _TclCompileClockClicksCmd =
	InterpCommand(interp, "::tcl::clock::clicks")->compileProc;
    _TclCompileClockReadingCmd =
	InterpCommand(interp, "::tcl::clock::seconds")->compileProc;

    /* Compatible < 8.6.7 versions: */
    if (!_TclCompileClockClicksCmd || !_TclCompileClockReadingCmd) {
	_TclCompileClockClicksCmd = _TclCompileBasicMin0ArgCmd;
	_TclCompileClockReadingCmd = _TclCompileBasicMin0ArgCmd;
    }

    _ClockClicksObjCmd =
	InterpCommand(interp, "::tcl::clock::clicks")->objProc;
    _ClockMillisecondsObjCmd =
	InterpCommand(interp, "::tcl::clock::milliseconds")->objProc;
    _ClockMicrosecondsObjCmd =
	InterpCommand(interp, "::tcl::clock::microseconds")->objProc;

    /* Create compiling ensemble command */
    (void)Tcl_CreateObjCommand(interp, "::tcl::namespace::ensemble-compile",
		TclpCompileEnsemblObjCmd, NULL, NULL);

    /* Icrement env-epoch if env variable changed */
    Tcl_TraceVar2(interp, "env", NULL,
	    TCL_GLOBAL_ONLY | TCL_TRACE_WRITES | TCL_TRACE_UNSETS |
	    TCL_TRACE_ARRAY, EnvEpochTraceProc, NULL);

}
