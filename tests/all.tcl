# load library:
source [file dirname [file dirname [info script]]]/lib/loader.tcl

cd $::env(TEMP)
# invoke test suite:
source [file dirname [info script]]/clock.test