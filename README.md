# Simulator For University Operating Systems Class

This system simulates the operations that an OS would perform while running other applications or IO operations. This simulator focuses on a single core system, that is able to run IO operations in parallel to run operations, but only one run operation is allowed into the system at any given time.

Along with the above, this simulator specifically handles:

* Process Scheduling
* Memory Management
* Threaded IO operations

## Installation

To run the system you will need a gcc compiler, then clone the repo
```
git clone <this repo>
cd sim_os
```

Then build the system:
```
make -f sim_mf
```

## Running

The simulator uses config files, one of which is included in the repo. These config files have nine editable fields, which are the following:

|Attribute|Type|
| --------------------- | -------------------------------------- |
| Version/Phase         | (float)                                |
| File Path             | (Path to metadata file)                |
| CPU Scheduling Code   | (Code for scheduling algorithm to run) |
| Quantum Time          | (Quantum for scheduling algorithm)     |
| Memory Available      | (Total memory in the system)           |
| Processor Cycle Time  | (Run operation cycle time)             |
| I/O Cycle Time        | (I/O operation cycle time)             |
| Log To                | (Logging method)                       |
| Log File Path         | (Path to Logging File)                 |

Each of which controls a different aspect of the simulator. This file also requires a metadata file that has a list of processes with operations to run. Two are included in the repo above and can be used as examples for creating other metadata files. 

Once you have created your own, or modified the existing files, to run the system:
```
./sim <config file>.cnf
```

This should then run the simulator using the given config and metadata file.
