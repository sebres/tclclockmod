puts "Testing with [info nameofexecutable] ..."

puts "Load library ..."
# load library:
source [file dirname [file dirname [info script]]]/lib/loader.tcl

cd $::env(TEMP)

puts "Test ..."
# invoke test suite:
source [file dirname [info script]]/clock.test