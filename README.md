[![Build Status](https://travis-ci.org/flightaware/tclclockmod.svg?branch=master)](https://travis-ci.org/flightaware/tclclockmod)

          _____   _  ___ _         _   __  __         _ 
         |_   _|_| |/ __| |___  __| |_|  \/  |___  __| |
           | |/ _| | (__| / _ \/ _| / / |\/| / _ \/ _` |
           |_|\__|_|\___|_\___/\__|_\_\_|  |_\___/\__,_|
           v.8.6.7-03                         2018/12/03
                                                       

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

Performance ?
-------------

Current performance increase (in comparison vs the tcl-core clock):

Function | Performance increase | tclclockmod | tcl8.6-clock
-------- | -------------------- | ----------- | ------------
`clock format` | 15 - 20 times faster | 0.27 - 4.28 µs/# | 5.45 - 45 µs/#
`clock scan -format` | 40 - 70 times (up to 100 times faster \*)<br/><sub>\* some previously extremely slow scans</sub> | 0.44 - 1.72 µs/# | 21 - 120 µs/#
`clock scan` (freescan) | 15 - 20 times | 0.51 - 5.84 µs/# | 12 - 77 µs/#
`clock add` | 50 - 90 times | 0.31 - 0.68 µs/# | 15 - 45 µs/#

The difference is much more larger, if the tests are running multi-threaded with parasitic load.

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

* GH-actions:
  - [![CI-nix](https://github.com/sebres/tclclockmod/actions/workflows/ci-nix.yml/badge.svg)](https://github.com/sebres/tclclockmod/actions/workflows/ci-nix.yml)

Differences from base clock
===========================

clock unixtime
--------------

Same as "clock scan" except:

* If the -timezone argument is an empty string, it uses timezone UTC
* If the input string is an integer, it assumes it's already a UNIX epoch timestamp and returns it unchanged
* If the input sting is not a valid time, it returns 0 rather than generating an error

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
