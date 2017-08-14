if {[catch {
  load tclclockmod86t
}]} {
  load tclclockmod86gt
}
source {..\..\lib\clock.tcl}
proc clock args {
  set cmdmap [dict create]
  foreach cmd {add clicks format microseconds milliseconds scan seconds configure} {
    dict set cmdmap $cmd ::tcl::clock::$cmd
  }
  namespace inscope ::tcl::clock [list namespace ensemble create -command \
    [uplevel 1 [list ::namespace origin [::lindex [info level 0] 0]]] \
    -map $cmdmap]
  ::tcl::namespace::ensemble-compile "::clock"

  uplevel 1 [info level 0]
}
