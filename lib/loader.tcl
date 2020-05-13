# save clockmode path:
set ::tcl::clock::LibDir [file dirname [info script]]

# rewrite clock ensemble:
proc ::clock args {
  if {$::tcl_platform(platform) ne {windows}} {
    set lib "unix"
    set name "lib"
  } else {
    set lib "win"
    set name ""
  }
  append name tclclockmod * [info sharedlibextension]
  # first try from lib directory (like installed):
  set lib [glob -nocomplain [file join $::tcl::clock::LibDir $name]]
  # second try find library from current directory (debug, release, platform etc.),
  # hereafter in path relative current lib (like unistalled):
  if {![llength $lib]} {
    foreach plib [list \
      [if {[info exists ::BUILDDIR]} {set ::BUILDDIR} else pwd] \
      [file dirname $::tcl::clock::LibDir] \
    ] {
      # now from unix, win, Release:
      foreach lib [list {} Release* $lib [file join $lib Release*]] {
        #puts "**** try $plib / $lib -- [file join $plib $lib $name]"
        set lib [glob -nocomplain [file join $plib $lib $name]]
        #puts "==== $lib"
        if {[llength $lib]} break
      }
      if {[llength $lib]} break
    }
    if {![llength $lib]} {
      error "tclclockmod shared library not found relative \"[pwd]\"."
    }
  }
  # load library:
  load [lindex $lib 0]

  # overload new tcl-clock stubs:
  source [file join $::tcl::clock::LibDir clock.tcl]

  # and ensemble:
  set cmdmap [dict create]
  foreach cmd {add clicks format microseconds milliseconds scan seconds unixtime configure} {
    dict set cmdmap $cmd ::tcl::clock::$cmd
  }
  namespace inscope ::tcl::clock [list namespace ensemble create -command \
    [uplevel 1 [list ::namespace origin [::lindex [info level 0] 0]]] \
    -map $cmdmap]
  ::tcl::namespace::ensemble-compile "::clock"

  uplevel 1 [info level 0]
}
