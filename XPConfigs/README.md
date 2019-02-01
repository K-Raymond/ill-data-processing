# Config Files

## XPConfig.txt

Each row represents a detector indexed by the first column. Holds information relating to energy calibration, detector type, energy range, and so on. This file is most commonly used in scripts  with `TXPConfig`'s constructor:

`TXPConfig XPConfig = new TXPConfig( "./XPConfigs/XPConfig.txt" );`

## XPGeometry.txt

Contains information about the specific geometry of the array as related to the index number from XPConfig. This allows to translate the index to a physical detector, and crystal position. Geometry information is built into TXPConfig.

TODO: Add information related to the physical positions of the detectors themselves
