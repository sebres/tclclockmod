[![Build status](https://github.com/flightaware/tclclockmod/actions/workflows/linux-ci.yml/badge.svg)](https://github.com/flightaware/tclclockmod/actions/workflows/linux-ci.yml)

          _____   _  ___ _         _   __  __         _ 
         |_   _|_| |/ __| |___  __| |_|  \/  |___  __| |
           | |/ _| | (__| / _ \/ _| / / |\/| / _ \/ _` |
           |_|\__|_|\___|_\___/\__|_\_\_|  |_\___/\__,_|
           v.8.6.7-03                         2018/12/03
                                                       

## TclClockMod: the [fastest, most powerful](#performance-) Tcl clock engine written in C

What is this ?
==============

This is the source distribution of the Tcl clock extension: the [faster](#performance-) 
Tcl-module for the replacement of the standard "clock" ensemble of tcl.

You need to have your Tcl core compiled also.

This extension is a freely available open source package. You can do
virtually anything you like with it, such as modifying it, redistributing
it, and selling it either in whole or in part.  See the "license.terms"
file in the top-level distribution directory for complete information.

Now this clock-engine is a part of Tcl 8.7 / 9.0.

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

#### How the performance is measured:

Both tcl-core as well as tclclockmod has a file [tests-perf/clock.perf.tcl](./tests-perf/clock.perf.tcl) which can be used to compare the execution times of original clock and tclclockmod. It can be also simply performed from the tclsh, with and without loading of the module.<br/>
Here is a diff illustrating that (which amounted to almost 95x speed-up):
```diff
  % timerate -calibrate {}
  % clock scan "" -timezone :CET; clock scan "" -gmt 1; # warming up
  % timerate { clock scan "2009-06-30T18:30:00 CEST" -format "%Y-%m-%dT%H:%M:%S %z" -gmt 1 }
- 62.0972 µs/# 16094 # 16103.8 #/sec 999.392 net-ms
+ 0.654699 µs/# 1437085 # 1527419 #/sec 940.858 net-ms
```

Tcl compatibility:
=================

Although this clock-ensemble version is almost 100% compatible (except of some 
changes of the logic as regards the bug-fixing), but you should nevertheless
test it with your application.

The module is currently usable with latest Tcl 8.6th version (>= 8.6.6), but can
be used also with previous versions since 8.6.0 (note that some packages like 
"msgcat" should be upgraded in this case).

Since [TIP 688](https://core.tcl-lang.org/tips/doc/trunk/tip/688.md) (commits [GH/tcl/e736133f9c72](https://github.com/tcltk/tcl/commit/e736133f9c72a69186f1d6845b5fb52de03c23ab) or [CORE/tcl/7137ea11e9e343f6](https://core.tcl-lang.org/tcl/info/7137ea11e9e343f6)) this is a part of Tcl 8.7 / 9.0 and therefore fully compatible to newest core-tcl now, excepting few things (like `clock configure` -> `tcl::unsupported::clock::configure`).

Differences from base clock: `clock unixtime`
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
