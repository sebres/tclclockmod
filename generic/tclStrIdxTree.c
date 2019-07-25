/*
 * tclStrIdxTree.c --
 *
 *	Contains the routines for managing string index tries in Tcl.
 *
 *	This code is back-ported from the tclSE engine, by Serg G. Brester.
 *
 * Copyright (c) 2016 by Sergey G. Brester aka sebres. All rights reserved.
 *
 * See the file "license.terms" for information on usage and redistribution of
 * this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 * -----------------------------------------------------------------------
 *
 * String index tries are prepaired structures used for fast greedy search of the string
 * (index) by unique string prefix as key.
 *
 * Index tree build for two lists together can be explained in the following datagram
 *
 * Lists:
 *
 *	{Januar Februar Maerz April Mai Juni Juli August September Oktober November Dezember}
 *	{Jnr Fbr Mrz Apr Mai Jni Jli Agt Spt Okt Nvb Dzb}
 *
 * Index-Tree:
 *
 *	j		 0	 *	...
 *	 anuar		 1	 *
 *	 u		 0	 *	a		 0
 *	  ni		 6	 *	 pril		 4
 *	  li		 7	 *	 ugust		 8
 *	 n		 0	 *	 gt		 8
 *	  r		 1	 *	s		 9
 *	  i		 6	 *	 eptember	 9
 *	 li		 7	 *	 pt		 9
 *	f		 2	 *	oktober		10
 *	 ebruar		 2	 *	n		11
 *	 br		 2	 *	 ovember	11
 *	m		 0	 *	 vb		11
 *	 a		 0	 *	d		12
 *	  erz		 3	 *	 ezember	12
 *	  i		 5	 *	 zb		12
 *	 rz		 3	 *
 *	...
 *
 * Thereby value 0 shows pure group items (corresponding ambigous matches).
 * But the group may have a value if it contains only same values
 * (see for example group "f" above).
 *
 * StrIdxTree's are very fast, so:
 *    build of above-mentioned tree takes about 10 microseconds.
 *    search of string index in this tree takes fewer as 0.1 microseconds.
 *
 */

#include "tclInt.h"
#include "tclStrIdxTree.h"


/*
 *----------------------------------------------------------------------
 *
 * TclStrIdxTreeSearch --
 *
 *  Find largest part of string "start" in indexed tree (case sensitive).
 *
 *  Also used for building of string index tree.
 *
 * Results:
 *  Return position of UTF character in start after last equal character
 *  and found item (with parent).
 *
 * Side effects:
 *  None.
 *
 *----------------------------------------------------------------------
 */

const char*
TclStrIdxTreeSearch(
    TclStrIdxTree **foundParent, /* Return value of found sub tree (used for tree build) */
    TclStrIdx	  **foundItem,	 /* Return value of found item */
    TclStrIdxTree  *tree,	 /* Index tree will be browsed */
    const char	*start,		 /* UTF string to find in tree */
    const char	*end)		 /* End of string */
{
    TclStrIdxTree *parent = tree, *prevParent = tree;
    TclStrIdx  *item = tree->firstPtr, *prevItem = NULL;
    const char *s = start, *f, *e, *prevf = NULL;

    if (item == NULL) {
	goto done;
    }

    /* search in tree */
    do {
	e = item->pos;
	f = TclUtfFindEqualNCInLwr(s, end, e, e + item->len, &e);
	/* if something was found */
	if (f > s) {
	    /* if whole string was found */
	    if (f >= end) {
		start = f;
		goto done;
	    };
	    /* shift start string */
	    s = f;
	    /* if match item, go deeper as long as possible */
	    if (e - item->pos >= item->len && item->childTree.firstPtr) {
		/* save previuosly found item (if not ambigous) for
		 * possible fallback (few greedy match) */
		if (item->value != NULL) {
		    prevf = f;
		    prevItem = item;
		    prevParent = parent;
		}
		parent = &item->childTree;
		item = item->childTree.firstPtr;
		continue;
	    }
	    /* no children - return this item and current chars found */
	    start = f;
	    goto done;
	}

	item = item->nextPtr;

    } while (item != NULL);

    /* fallback (few greedy match) not ambigous (has a value) */
    if (prevItem != NULL) {
	item = prevItem;
	parent = prevParent;
	start = prevf;
    }

done:

    if (foundParent)
	*foundParent = parent;
    if (foundItem)
	*foundItem = item;
    return start;
}

void
TclStrIdxTreeFree(
    TclStrIdx *tree)
{
    while (tree != NULL) {
	TclStrIdx *t;
	Tcl_DecrRefCount(tree->key);
	if (tree->childTree.firstPtr != NULL) {
	    TclStrIdxTreeFree(tree->childTree.firstPtr);
	}
	t = tree, tree = tree->nextPtr;
	ckfree(t);
    }
}

/*
 * Several bidirectional list primitives
 */
static inline void
TclStrIdxTreeInsertBranch(
    TclStrIdxTree *parent,
    register TclStrIdx *item,
    register TclStrIdx *child)
{
    if (parent->firstPtr == child)
	parent->firstPtr = item;
    if (parent->lastPtr == child)
	parent->lastPtr = item;
    if ( (item->nextPtr = child->nextPtr) ) {
	item->nextPtr->prevPtr = item;
	child->nextPtr = NULL;
    }
    if ( (item->prevPtr = child->prevPtr) ) {
	item->prevPtr->nextPtr = item;
	child->prevPtr = NULL;
    }
    item->childTree.firstPtr = child;
    item->childTree.lastPtr = child;
}

static inline void
TclStrIdxTreeAppend(
    register TclStrIdxTree *parent,
    register TclStrIdx	   *item)
{
    if (parent->lastPtr != NULL) {
	parent->lastPtr->nextPtr = item;
    }
    item->prevPtr = parent->lastPtr;
    item->nextPtr = NULL;
    parent->lastPtr = item;
    if (parent->firstPtr == NULL) {
	parent->firstPtr = item;
    }
}


/*
 *----------------------------------------------------------------------
 *
 * TclStrIdxTreeBuildFromList --
 *
 * Build or extend string indexed tree from tcl list.
 * If the values not given the values of built list are indices starts with 1.
 * Value of 0 is thereby reserved to the ambigous values.
 *
 * Important: by multiple lists, optimal tree can be created only if list with
 * larger strings used firstly.
 *
 * Results:
 *  Returns a standard Tcl result.
 *
 * Side effects:
 *  None.
 *
 *----------------------------------------------------------------------
 */

int
TclStrIdxTreeBuildFromList(
    TclStrIdxTree *idxTree,
    int	       lstc,
    Tcl_Obj  **lstv,
    ClientData *values)
{
    Tcl_Obj  **lwrv;
    int i, offs, l, ret = TCL_ERROR;
    ClientData val;
    const char *s, *e, *f;
    TclStrIdx	*item;

    /* create lowercase reflection of the list keys */

    lwrv = ckalloc(sizeof(Tcl_Obj*) * lstc);
    if (lwrv == NULL) {
	return TCL_ERROR;
    }
    for (i = 0; i < lstc; i++) {
	lwrv[i] = Tcl_DuplicateObj(lstv[i]);
	if (lwrv[i] == NULL) {
	    return TCL_ERROR;
	}
	Tcl_IncrRefCount(lwrv[i]);
	lwrv[i]->length = Tcl_UtfToLower(TclGetString(lwrv[i]));
    }

    /* build index tree of the list keys */
    for (i = 0; i < lstc; i++) {
	TclStrIdxTree *foundParent = idxTree;
	offs = l = 0; /* parent offset and length */
	e = s = TclGetString(lwrv[i]);
	e += lwrv[i]->length;
	val = values ? values[i] : INT2PTR(i+1);

	/* ignore empty keys (impossible to index it) */
	if (lwrv[i]->length == 0) continue;

	item = NULL;
	if (idxTree->firstPtr != NULL) {
	    TclStrIdx  *foundItem;
	    f = TclStrIdxTreeSearch(&foundParent, &foundItem,
		idxTree, s, e);
	    /* if common prefix was found */
	    if (f > s) {
		/* ignore element if fulfilled or ambigous */
		if (f == e) {
		    continue;
		}
		/* if shortest key was found with the same value,
		 * just replace its current key with longest key */
		offs = TclStrIdxGetItemOffset(foundItem);
		l = foundItem->len + offs;
		if ( foundItem->value == val
		  && l <= lwrv[i]->length
		  && l <= (f - s) /* only if found item is covered in full */
		  && foundItem->childTree.firstPtr == NULL
		) {
		    Tcl_SetObjRef(foundItem->key, lwrv[i]);
		    foundItem->pos = TclGetString(foundItem->key) + offs;
		    foundItem->len = lwrv[i]->length - offs;
		    continue;
		}
		/* split tree (e. g. j->(jan,jun) + jul == j->(jan,ju->(jun,jul)) )
		 * but don't split by fulfilled child of found item ( ii->iii->iiii ) */
		if (l != (f - s)) {
		    /* first split found item (insert one between parent and found + new one) */
		    item = ckalloc(sizeof(*item));
		    if (item == NULL) {
			goto done;
		    }
		    Tcl_InitObjRef(item->key, foundItem->key);
		    item->pos = TclGetString(item->key) + offs;
		    l = f - s;
		    item->len = l - offs;
		    /* set value or mark as ambigous if not the same value of both */
		    item->value = (foundItem->value == val) ? val : NULL;
		    /* insert group item between foundParent and foundItem */
		    TclStrIdxTreeInsertBranch(foundParent, item, foundItem);
		    foundParent = &item->childTree;
		    /* correct offset and length of found item (relative item now) */
		    foundItem->pos += item->len;
		    foundItem->len -= item->len;
		} else {
		    /* the new item should be added as child of found item */
		    foundParent = &foundItem->childTree;
		}
	    }
	}
	/* append item at end of found parent */
	item = ckalloc(sizeof(*item));
	if (item == NULL) {
	    goto done;
	}
	item->childTree.lastPtr = item->childTree.firstPtr = NULL;
	Tcl_InitObjRef(item->key, lwrv[i]);
	/* use length of parent as offset of new item */
	item->pos = TclGetString(item->key) + l;
	item->len = lwrv[i]->length - l;
	item->value = val;
	TclStrIdxTreeAppend(foundParent, item);
    };

    ret = TCL_OK;

done:

    if (lwrv != NULL) {
	for (i = 0; i < lstc; i++) {
	    Tcl_DecrRefCount(lwrv[i]);
	}
	ckfree(lwrv);
    }

    if (ret != TCL_OK) {
	if (idxTree->firstPtr != NULL) {
	    TclStrIdxTreeFree(idxTree->firstPtr);
	}
    }

    return ret;
}


static void
StrIdxTreeObj_DupIntRepProc(Tcl_Obj *srcPtr, Tcl_Obj *copyPtr);
static void
StrIdxTreeObj_FreeIntRepProc(Tcl_Obj *objPtr);
static void
StrIdxTreeObj_UpdateStringProc(Tcl_Obj *objPtr);

Tcl_ObjType StrIdxTreeObjType = {
    "str-idx-tree",		    /* name */
    StrIdxTreeObj_FreeIntRepProc,   /* freeIntRepProc */
    StrIdxTreeObj_DupIntRepProc,    /* dupIntRepProc */
    StrIdxTreeObj_UpdateStringProc, /* updateStringProc */
    NULL			    /* setFromAnyProc */
};

Tcl_Obj*
TclStrIdxTreeNewObj()
{
    Tcl_Obj *objPtr = Tcl_NewObj();
    objPtr->internalRep.twoPtrValue.ptr1 = NULL;
    objPtr->internalRep.twoPtrValue.ptr2 = NULL;
    objPtr->typePtr = &StrIdxTreeObjType;
    /* return tree root in internal representation */
    return objPtr;
}

static void
StrIdxTreeObj_DupIntRepProc(Tcl_Obj *srcPtr, Tcl_Obj *copyPtr)
{
    /* follow links (smart pointers) */
    if ( srcPtr->internalRep.twoPtrValue.ptr1 != NULL
      && srcPtr->internalRep.twoPtrValue.ptr2 == NULL
    ) {
	srcPtr = (Tcl_Obj*)srcPtr->internalRep.twoPtrValue.ptr1;
    }
    /* create smart pointer to it (ptr1 != NULL, ptr2 = NULL) */
    Tcl_InitObjRef(*((Tcl_Obj **)&copyPtr->internalRep.twoPtrValue.ptr1),
	srcPtr);
    copyPtr->internalRep.twoPtrValue.ptr2 = NULL;
    copyPtr->typePtr = &StrIdxTreeObjType;
}

static void
StrIdxTreeObj_FreeIntRepProc(Tcl_Obj *objPtr)
{
    /* follow links (smart pointers) */
    if ( objPtr->internalRep.twoPtrValue.ptr1 != NULL
      && objPtr->internalRep.twoPtrValue.ptr2 == NULL
    ) {
	/* is a link */
	Tcl_UnsetObjRef(*((Tcl_Obj **)&objPtr->internalRep.twoPtrValue.ptr1));
    } else {
	/* is a tree */
	TclStrIdxTree *tree = (TclStrIdxTree*)&objPtr->internalRep.twoPtrValue.ptr1;
	if (tree->firstPtr != NULL) {
	    TclStrIdxTreeFree(tree->firstPtr);
	}
	objPtr->internalRep.twoPtrValue.ptr1 = NULL;
	objPtr->internalRep.twoPtrValue.ptr2 = NULL;
    }
    objPtr->typePtr = NULL;
};

static void
StrIdxTreeObj_UpdateStringProc(Tcl_Obj *objPtr)
{
    /* currently only dummy empty string possible */
    objPtr->length = 0;
    objPtr->bytes = tclEmptyStringRep;
};

TclStrIdxTree *
TclStrIdxTreeGetFromObj(Tcl_Obj *objPtr) {
    /* follow links (smart pointers) */
    if (objPtr->typePtr != &StrIdxTreeObjType) {
	return NULL;
    }
    if ( objPtr->internalRep.twoPtrValue.ptr1 != NULL
      && objPtr->internalRep.twoPtrValue.ptr2 == NULL
    ) {
	objPtr = (Tcl_Obj*)objPtr->internalRep.twoPtrValue.ptr1;
    }
    /* return tree root in internal representation */
    return (TclStrIdxTree*)&objPtr->internalRep.twoPtrValue.ptr1;
}

/*
 * Several debug primitives
 */
#if 0
/* currently unused, debug resp. test purposes only */

void
TclStrIdxTreePrint(
    Tcl_Interp *interp,
    TclStrIdx  *tree)
{
    Tcl_Obj *obj[2];
    const char *s;
    Tcl_InitObjRef(obj[0], Tcl_NewStringObj("::puts", -1));
    while (tree != NULL) {
	offs = TclStrIdxGetItemOffset(tree);
	Tcl_InitObjRef(obj[1], Tcl_ObjPrintf("%*s%.*s\t:%d",
		offs, "", tree->len, tree->pos, PTR2INT(tree->value)));
	Tcl_PutsObjCmd(NULL, interp, 2, obj);
	Tcl_UnsetObjRef(obj[1]);
	if (tree->childTree.firstPtr != NULL) {
	    TclStrIdxTreePrint(interp, tree->childTree.firstPtr);
	}
	tree = tree->nextPtr;
    }
    Tcl_UnsetObjRef(obj[0]);
}


int
TclStrIdxTreeTestObjCmd(
    ClientData clientData, Tcl_Interp *interp,
    int objc, Tcl_Obj *const objv[])
{
    const char *cs, *cin, *ret;

    static const char *const options[] = {
	"index", "puts-index", "findequal",
	NULL
    };
    enum optionInd {
	O_INDEX,  O_PUTS_INDEX, O_FINDEQUAL
    };
    int optionIndex;

    if (objc < 2) {
	Tcl_WrongNumArgs(interp, 1, objv, "");
	return TCL_ERROR;
    }
    if (Tcl_GetIndexFromObj(interp, objv[1], options,
	"option", 0, &optionIndex) != TCL_OK) {
	Tcl_SetErrorCode(interp, "CLOCK", "badOption",
		Tcl_GetString(objv[1]), NULL);
	return TCL_ERROR;
    }
    switch (optionIndex) {
    case O_FINDEQUAL:
	if (objc < 4) {
	    Tcl_WrongNumArgs(interp, 1, objv, "");
	    return TCL_ERROR;
	}
	cs = TclGetString(objv[2]);
	cin = TclGetString(objv[3]);
	ret = TclUtfFindEqual(
	    cs, cs + objv[1]->length, cin, cin + objv[2]->length);
	Tcl_SetObjResult(interp, Tcl_NewIntObj(ret - cs));
    break;
    case O_INDEX:
    case O_PUTS_INDEX:

    if (1) {
	Tcl_Obj **lstv;
	int i, lstc;
	TclStrIdxTree idxTree = {NULL, NULL};
	i = 1;
	while (++i < objc) {
	    if (TclListObjGetElements(interp, objv[i],
		    &lstc, &lstv) != TCL_OK) {
		return TCL_ERROR;
	    };
	    TclStrIdxTreeBuildFromList(&idxTree, lstc, lstv, NULL);
	}
	if (optionIndex == O_PUTS_INDEX) {
	    TclStrIdxTreePrint(interp, idxTree.firstPtr);
	}
	TclStrIdxTreeFree(idxTree.firstPtr);
    }
    break;
    }

    return TCL_OK;
}

#endif

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 4
 * fill-column: 78
 * End:
 */
