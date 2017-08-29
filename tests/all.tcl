puts "Testing with [info patchlevel] from [info nameofexecutable] ..."

puts "Load library ..."
# load library:
source [file dirname [file dirname [info script]]]/lib/loader.tcl

if {[catch {
  cd $::env(TEMP)
}]} {
  cd /tmp/
}

puts "Test ..."
# invoke test suite:
source [file dirname [info script]]/clock.test