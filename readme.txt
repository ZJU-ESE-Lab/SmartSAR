# README.TXT

This project is an HC12 ANSI-C project.

This example is running the "Demo" program on:
-the CodeWarrior HC12 Simulator, (choose "Demo (Simulation)" target),
-ICD12 P&E cables interface (ICD12, Cable12, Cable12HS, Multilink) (choose "Demo (P&E Multilink)" target),
-BDIK Abatron BDI1000 interface (choose "Demo (Abatron bdi1000)" target).


This program shows a combination of features of the debugger:

-when "Simulation" debugging with "SampleS12.io" (SCIs, RTIs, io PORTB pure simulation),
"Visualization Tool" interaction with program (Subscription / Each access refresh),
and usage of Terminal component in terminal emulation mode.

-when "real hardware" debugging (in BDM mode, via P&E cable or Abatron BDI), 
with "Visualization Tool" interaction (Memory/ periodical refresh), 
and usage of Terminal component in real terminal mode.


Folders used:
- Sources: contains your sources. It contains a 'main.c' as a base and 'isr_vectors'
  for cpu vectors mapping.
- Debugger Cmd Files: Here you can place your own specific debugger command
  files.
- Startup Code: Contains the startup code of the application. You may adapt it
  to your own needs, but in such a case make sure that you create a local copy.
- Prm: Linker parameter file used for linking. Note that the file used for the linker
  is specified in the Linker Preference Panel itself (<ALT-F7> or Edit->{Current Build Target Name} settings,
  while the project window is opened).
- Libs: contains the ANSI library to be linked with.
- Debugger Project File: This *.ini file is passed to the debugger/simulator
  as configuration file. It contains various target interface settings, and path
  information as well. This file can be edited from the simulator/debugger e.g. using
  File->Configuration in the simulator/debugger.

This project includes a burner command file (.bbl) in order to generate a 
logical SRecord file (.s19) and a physical/linear SRecord file (.phy).  
Remove this file from prm folder if no SRecord is needed.


Hardware Notes:
===============

MC9S12DP256 EVB / EVB912DP256 - (Motorola):

The board is monitored via the BDM interface (J19 - BDM IN connector).

Jumper settings: jumpers J12 and J13 should be set to 1-2 position.

ALSO:
When using an Abatron BDI1000, please connect the BDI to the PC COM2 port. Indeed, the project
uses COM1 for Terminal communication with the microcontroller.
An RS-232 serial communication cable should be connected between the EVB com port (P1) and the PC COM1 port.

WARNING:
When flashing the board with this project application, any factory pre-programmed 
application (if existing) will be entirely erased.
To get the factory pre-programmed application (if any, like a monitor), please contact Motorola.


Metrowerks
