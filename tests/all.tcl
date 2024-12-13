# prepare:
set TEST_OPTIONS {}
if {[lsearch [namespace children] ::tcltest] == -1} {
  package require tcltest
  namespace import tcltest::*
  ::tcltest::configure {*}[set TEST_OPTIONS $::argv]
  proc ::tcltest::exit {args} {}
}

puts [outputChannel] "Testing with [info patchlevel] from [info nameofexecutable] ..."

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
proc ::tcltest::__ReportToParent {total passed skipped failed skippedLst args} {
  array set ::TestSummary [list \
    Total $total Passed $passed Skipped $skipped Failed $failed skippedBecauseLst $skippedLst args $args]
  incr ::TestSummary(TotFailed) $failed
  ::tcltest::__ReportSummary
  ::tcltest::ReportedFromChild $total $passed $skipped $failed $skippedLst {*}$args
}
proc ::tcltest::__SortFiles {lst} {
  set slst {}
  foreach f $lst {
    lappend slst [list [file rootname $f] $f]
  }
  set lst {}
  foreach f [lsort -dictionary -index 0 $slst] {
    lappend lst [lindex $f 1]
  }
  return $lst
}

set TESTDIR [file normalize [file dirname [info script]]]
set BUILDDIR [pwd]
if {![::tcl::pkgconfig get debug]} { # allow to load lib from current directory in debug:
  # switch to temp directory:
  if {[catch {
    cd $::env(TEMP)
  }]} {
    cd /tmp/
  }
}
set GLOB_OPTIONS {
  puts [outputChannel] "  Load library ..."
  # load library:
  source [file dirname [file dirname $TESTFILE]]/lib/loader.tcl
  # test it is really new version:
  clock format -now -format "%Es" -gmt 1
  puts [outputChannel] "  Test ..."
}
foreach testfile [::tcltest::__SortFiles [::tcltest::GetMatchingFiles $TESTDIR]] {
  # prepare single run:
  set ::TestSummary(File) [file root [file tail $testfile]]
  incr ::tcltest::numTestFiles
  puts -nonewline [outputChannel] [set msg "== ${::tcltest::numTestFiles}. $::TestSummary(File) "]
  puts [outputChannel] [string repeat = [expr {80-[string length $msg]}]]
  set slave [interp create]
  interp eval $slave [package ifneeded tcltest $tcltest::Version]
  $slave eval {namespace import tcltest::*}
  interp alias $slave ::tcltest::ReportToParent {} ::tcltest::__ReportToParent
  $slave eval [list set TESTFILE [file join $TESTDIR $testfile]]
  $slave eval [list set BUILDDIR $BUILDDIR]
  $slave eval [list ::tcltest::configure {*}$TEST_OPTIONS]
  $slave eval $GLOB_OPTIONS
  # invoke test suite:
  $slave eval {
    source $TESTFILE
  }
  interp delete $slave
}

# commit:
puts [outputChannel] "\n[string repeat ==== 20]"
::tcltest::cleanupTests 1

# if calling direct:
if {[info exists ::argv0] && [file tail $::argv0] eq [file tail [info script]]} {
  if {$::TestSummary(TotFailed)} {
    puts stderr "\n[string repeat ** 20]"
    puts stderr "** ERROR: totally $::TestSummary(TotFailed) test(s) failed"
    puts stderr [string repeat ** 20]
    exit 1
  }
  puts [outputChannel] "\nOK."
}
