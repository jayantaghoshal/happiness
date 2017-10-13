Car Config
============

The Car Config is a set of libraries to enable reading and writing Car Config parameters. libcarconfig_reader is used by applications for reading configuration data. libcarconfig_writer is used by CarConfigUpdater to write received carconfig to disk.

Backend File Format
-------------

The backend file format is based on the CSV file format according to the following format, three values seperated by comma character
and a newline character to signify next parameter:

[RAW VALUE],[SUBSTITUTE VALUE],[FLAG]\n

* Raw format is the actual format from flexray.
* Substitute value is the previous good value.
* Flag identifies the status of the RAW value, good(0) or invalid(1).

The line number signifies the actual parameter ID, so ID 1 equals line number 0. The file MUST always have the correct number of lines, in the case
of the IHU case 504 lines.

Example:

255,33,1 <br/>
22,55,0 <br/>
11,44,0 <br/>
