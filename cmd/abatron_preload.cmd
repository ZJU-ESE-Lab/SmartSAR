// Before load the commands written below will be executed

// The following commands must be enabled to load in flash with the BDI.

// reset the device to get default settings
RESET
// initialize flash programming process
bdi flash.erase addr=308000
bdi flash.erase addr=348000
bdi flash.erase addr=388000
bdi flash.erase addr=3C8000
bdi flash.load
