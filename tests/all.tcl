puts "Testing with [info patchlevel] from [info nameofexecutable] ..."

puts "Load library ..."
# load library:
source [file dirname [file dirname [info script]]]/lib/loader.tcl
# test it is really new version:
clock format -now -format "%Es" -gmt 1

if {[catch {
  cd $::env(TEMP)
}]} {
  cd /tmp/
}

# prepare:
package require tcltest
namespace import tcltest::*
# register callback:
set ::TestSummary(Failed) 0
proc ::tcltest::ReportToMaster {Total Passed Skipped Failed args} {
  array set ::TestSummary [list \
    Total $Total Passed $Passed Skipped $Skipped Failed $Failed args $args]
}

puts "Test ..."
# invoke test suite:
source [file dirname [info script]]/clock.test

# if calling direct:
if {[info exists ::argv0] && [file tail $::argv0] eq [file tail [info script]]} {
  if {$::TestSummary(Failed)} {
    puts stderr "\n[string repeat ** 20]"
    puts stderr "** ERROR: $::TestSummary(Failed) test(s) failed"
    puts stderr [string repeat ** 20]
    exit 1
  }
}
