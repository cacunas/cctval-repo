cctval-repo
===========

This repository consists of the source code produced with Geant4 toolkit during my internship at CCTVAL.

The purpose of the current proyect ('t02') is to write a simple implementation of a scintillator using 
the specs of the Prelude420 crystal (whose datasheet is included in the repo for reference and comparison 
with the performance showed in the program).

 At the time of the update of this README, the program is working with a rudimentary implementation of the 
physical spects of the crystal, and the recollection of data is performed by two sensors on the opposite 
extremes of the crystal. The physical layout is simply a gun shooting gamma photons at 511 keV to a 5-cm-long 
scintillator crystal with two sensors on its ends (they were intended to be on the left and right, but with 
the visualization driver it shows that in reality they are on the top and bottom).
