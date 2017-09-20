Cedric
==========================

Cedric is a set of libraries and helper functions meant to be reused across components in the Volvo Cars IHU project. The core folder contains all the developer facing APIs, the other folders contains internal functionality that is not meant to be used by consumers of Cedric.

Folder structure
-------------
* core - Cedric user APIs.
* carconfig - Internal library functionality for reading/writing carconfig parameters.
* carconfig_onshot - Application that loads the carconfig file from disk to shared memory.
* carconfig_updater - Application that reads carconfig frames from Flexray and writes to disk, also responsible for setting diagnostic messages<br/>
  and restating the node when updated carconfig configuration has been loaded.
* carconfig_qmlplugin - The qmlplugin which is the api between the HMI and the backend for car config.
* localconfig_qmlplugin - The qmlplugin which is the api between the HMI and the backend for local config.

Compiling Cedric
-------------
Some of the software here uses git submodules, so you have to do:

    git clone ...
    git submodule init
    git submodule update

to get it to compile.

Core modules
-------------

### Carconfig
Carconfig is a library for reading vehicle configuration parameters (carconfig ) received on FlexRay.

### File monitor
File monitor uses inotify to monitor change on a given file. It creates a background thread for actual monitoring and notification (notification is always invoked on background thread.

### HWID 
HWID is a library for getting and setting the ID of the IHU hardware.

### Localconfig
HWID is a library for reading local configuration parameters (Localconfig).

### Nodestate
Nodestate is a wrapper library for accessing Genivi Node State Manager(NSM). The native API to NSM is D-Bus based, but to simplify things for the developer a c++ API has been created. The main functionality for Nodestate from a developer perspective is to subscribe to shutdown messages. When then system starts to shut down NSM informs all shutdown clients that they should prepare for shutdown, turn of functionality so that when the system shuts down the services shut down quickly. The shutdown could be aborted, the clients are then expected to go back to full functionality.

Nodestate also has functionality for setting session states.

### Reset
Factory Reset/Global Reset

There are 4 main types of reset:

1. Profile Reset ( settings changed for the current profile will be set to default )
2. All profile Reset ( settings made to any profile will be set to default )
3. Car Reset ( global settings not related to a profile will be reset. Profile settings will NOT be reset )
4. Factory Reset ( Resets all settings to defaults, essentially a Car Reset + All Profile Reset )

The main mechanism for reset is to erase settings memory and as a result an application unable to load settings will be responsible for providing its own default values. An application must also be prepared to re-read their settings upon trigger and adjust to the new settings (can be due to several reasons: diagnostics, driver profile change, reset, etc)
