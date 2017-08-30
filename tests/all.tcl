puts "Testing with [info patchlevel] from [info nameofexecutable] ..."

# prepare:
package require tcltest
namespace import tcltest::*
proc ::tcltest::exit {args} {}

# register callback:
array set ::TestSummary {Total 0 Passed 0 Skipped 0 Failed 0 TotFailed 0 skippedBecauseLst {} File {}}
proc ::tcltest::__ReportSummary {} {
  variable numTestFiles
  # single test-file:
  puts -nonewline [outputChannel] "${numTestFiles}. $::TestSummary(File):"
  foreach index [list "Total" "Passed" "Skipped" "Failed"] {
    puts -nonewline [outputChannel] "\t$index\t$::TestSummary($index)"
  }
  puts [outputChannel] ""
  # its constraints:
  if {[llength $::TestSummary(skippedBecauseLst)] > 0} {
    array set skippedBecause $::TestSummary(skippedBecauseLst)
    set constraintList [array names skippedBecause]
    puts [outputChannel] "   Skipped:"
    foreach constraint [lsort $constraintList] {
      puts [outputChannel] "\t$skippedBecause($constraint)\t$constraint"
    }
  }
  puts [outputChannel] ""
}
proc ::tcltest::__ReportToMaster {total passed skipped failed skippedLst args} {
  array set ::TestSummary [list \
    Total $total Passed $passed Skipped $skipped Failed $failed skippedBecauseLst $skippedLst args $args]
  incr ::TestSummary(TotFailed) $failed
  incr ::tcltest::numTestFiles
  ::tcltest::__ReportSummary
  ::tcltest::ReportedFromSlave $total $passed $skipped $failed $skippedLst {*}$args
}
# switch to temp directory:
if {[catch {
  cd $::env(TEMP)
}]} {
  cd /tmp/
}

set GLOB_OPTIONS {
  puts "Load library ..."
  # load library:
  source [file dirname [file dirname $TESTFILE]]/lib/loader.tcl
  # test it is really new version:
  clock format -now -format "%Es" -gmt 1
  puts "Test ..."
}
foreach {testfile testadd options} {
  clock.test {} {}
} {
  # prepare single run:
  set ::TestSummary(File) [file root $testfile]$testadd
  set slave [interp create]
  interp eval $slave [package ifneeded tcltest $tcltest::Version]
  $slave eval {namespace import tcltest::*}
  interp alias $slave ::tcltest::ReportToMaster {} ::tcltest::__ReportToMaster
  $slave eval [list set TESTFILE [file join [file dirname [info script]] $testfile]]
  $slave eval $GLOB_OPTIONS$options
  # invoke test suite:
  $slave eval {
    source $TESTFILE
  }
  interp delete $slave
}

# commit:
puts "\n[string repeat ==== 20]"
::tcltest::cleanupTests 1

# if calling direct:
if {[info exists ::argv0] && [file tail $::argv0] eq [file tail [info script]]} {
  if {$::TestSummary(TotFailed)} {
    puts stderr "\n[string repeat ** 20]"
    puts stderr "** ERROR: totally $::TestSummary(TotFailed) test(s) failed"
    puts stderr [string repeat ** 20]
    exit 1
  }
  puts "\nOK."
}
