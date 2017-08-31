
          _____   _  ___ _         _   __  __         _ 
         |_   _|_| |/ __| |___  __| |_|  \/  |___  __| |
           | |/ _| | (__| / _ \/ _| / / |\/| / _ \/ _` |
           |_|\__|_|\___|_\___/\__|_\_\_|  |_\___/\__,_|
           v.8.6.7-01                         2017/08/30
                                                       

## TclClockMod: the fastest, most powerful Tcl clock engine written in C

What is this ?
==============

This is the source distribution of the Tcl clock extension: the faster 
Tcl-module for the replacement of the standard "clock" ensemble of tcl.

You need to have your Tcl core compiled also.

This extension is a freely available open source package. You can do
virtually anything you like with it, such as modifying it, redistributing
it, and selling it either in whole or in part.  See the "license.terms"
file in the top-level distribution directory for complete information.

How to compile ?
----------------

Only Unix-like and Windows platforms are supported at the moment. Depending
on your platform (Unix-like or Windows) go to the appropriate directory
(unix or win) and start with the README file. Macintosh platform is supported
similar way the Tcl core does it also.

How to use ?
------------

```tcl
package require tclclockmod
clock format -now
```

Tcl compatibility:
=================

Although this clock-ensemble version is almost 100% compatible (except of some 
changes of the logic as regards the bug-fixing), but you should nevertheless
test it with your application.

The module is currently usable with latest Tcl 8.6th version (>= 8.6.6), but can
be used also with previous versions since 8.6.0 (note that some packages like 
"msgcat" should be upgraded in this case).

Code status (CI):
-----------------

* travis-ci.org:
  - [![Build Status](https://travis-ci.org/sebres/tclclockmod.svg?branch=master)](https://travis-ci.org/sebres/tclclockmod) (master)
  - [![Build Status](https://travis-ci.org/sebres/tclclockmod.svg)](https://travis-ci.org/sebres/tclclockmod) (latest)


Contact:
--------

### Bugs, feature requests, discussions?
Use github [issue-tracker](https://github.com/sebres/tclclockmod).

### You just appreciate this program:
send kudos to the original author ([Sergey G. Brester](mailto:github@sebres.de)).

Thanks:
-------

- FlightAware for the inspiration for me to write it (due to their bounty-program).
- TCT and all other contributors for the great language (long live Tcl!).


License:
--------

See the file "license.terms" for information on usage and redistribution of
this file, and for a DISCLAIMER OF ALL WARRANTIES.
