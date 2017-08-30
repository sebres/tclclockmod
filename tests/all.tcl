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

puts "Test ..."
# invoke test suite:
source [file dirname [info script]]/clock.test