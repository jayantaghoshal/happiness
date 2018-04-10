# Carconfig


The Car Config is a set of libraries to enable reading and writing Car Config parameters. 

* carconfig - Libraries used to read car and write configs. libcarconfig_reader is used by applications for reading configuration data. libcarconfig_writer is used by CarConfigUpdater to write received carconfig to disk.
* carconfig_updater - Servie used to receive new parameters from CEM, store them in IHU, provide diagnostics and forward parameters to VIP.
* carconfig_java_lib - Java library to read car configs.
* tools
  * Code generator to create carconfig enumerations from signal DB.
  * Command line utilities for manually overriding car config.


## Usage in native application code

Android.bp

    shared_libs: ["libcarconfig_reader"]

TODO: Selinux

Example.cpp

    #include <carconfig.h>
    #include <cc_parameterlist.h>


    auto car_config_curvespeedsupport= carconfig::getValue<CC148_CurveSpeedSupportType>();

    const bool is_enabled_ =
        (car_config_curvespeedsupport == CC148_CurveSpeedSupportType::With_Curve_Speed_Warning_without_Curve_Speed_Assist);


## Usage in Java applications

TODO

## Default values

Taken during build from **carconfig-default.csv** 

This file is inteded for development only. It is OK to modify to make sure default values enable as much functionality as possible.  


### Default values backend file format

The backend file format is based on the CSV file format according to the following format, three values seperated by comma character and a newline character to signify next parameter:

[RAW VALUE],[SUBSTITUTE VALUE],[FLAG]\n

Raw format is the actual format from flexray.
Substitute value is the previous good value. Flag identifies the status of the RAW value, good(0) or invalid(1). The difference between raw and substitute is mainly used for gateway of carconfig, most applications shouldn't have to care about this.

The line number signifies the actual parameter ID, so ID 1 equals line number 0. The file MUST always have the correct number of lines, in the case of the IHU case 504 lines.

## Overriding car config manually

On engineering builds you can run **/vendor/bin/changecarconfig $PARAMNR $VALUE** and restart your services or restart the whole IHU.

NOTE: If you are testing in a rig or vehicle with CEM, you must disable carconfig_updater to avoid CEM parameters overriding your manually set parameters (see Update Handling below). This is easiest done by simply renaming **/vendor/bin/carconfig-updater** to something else.


## Update handling

On start up, carconfig will try to receive new parameters from CEM.
This is handled by the service **carconfig_updater**, using the binary **/vendor/bin/carconfig-updater**

*If the parameters differ, the parameters on the IHU will be overwritten and the IHU automatically rebooted.*

Before parameters are received, the defaults will be used.

## Parameterlist code generation
**cc_parameterlist.h** is auto generated during build based on **tools/carconfigspec.xls**,  it can be found in the out-directory after a successful build.