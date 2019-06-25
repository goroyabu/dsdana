## Installation

```
$ make
$ make install
$ source this.sh
```

## Usage

```
$ sh run_ana.sh example.root
```

If you want a hittree with detailed information,

```
$ sh run_ana_detail.sh example.root
```

## Modules

### Modules for analysis
 - ApplyDatabase
 - MergeAdjacent
 - CoupleHit

See 'Details of Analysis'

### Modules for Data IO
 - ReadEventtree
 - ReadDatabase
 - WriteHittree
 - WriteHittreeAll

See 'Data Format'

<a id="ana"></a><a href="#ana"></a>
## Details of Analysis 

### ApplyDatabase
Applying a database to convert ADC signals into energy and positional information.



### MergeAdjacent
Merging signals readout from adjacent strips.

1. Cut signals lower than energy thresholds

	- Energy thresholds are 5 keV for Al-side and 10 keV for Pt-side as default.
	- Energy thresholds are defined as 'ethre' in the database file.

2. Merge signals readout from adjacent strips
	- "Adjacent strips" means they have the same 'detid' and the gap of 'stripid' is 1.

3. Determine information of the merged signal

	- Energy information is sum of energy of each strips. 
	- Positional information is a position of the strip with maximum energy.
	- Width information is same as a strip pitch.


### CoupleHit
Coupling signals of both sides into information of 1 photon.

1. Classify events by numbers of signals on 1 side
2. Determine information of 1 photon
	- Energy information 
		- epi_lv3   : average of energy on each sides.
		- epi_x_lv3 : energy on Pt-side
		- epi_y_lv3 : energy on Al-side
	- Positional information
		- pos_x_lv3 : position of Pt-side
		- pos_y_lv3 : position of Al-side
		- pos_z_lv3 : position of the detector plane (0 as default)
		

## DataFormat

[Detail](DataFormat.md)

