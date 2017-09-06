# FDX Client

This module contains:

* Client implementing the Vector FDX protocol over UDP sockets
* Parser for the FDX Description file to create a simple mapping from signal names to groupIDs.
* Parser combining the FDX Description file with data elements to create signal scaling functions.


## CANoe FDX (Fast Data eXchange) is a protocol to exchange data with a Vector CANoe simulation over UDP

    For detailed information, refer to Vector documentation (included with installation of CANoe).
        CANoe FDX Protocol Version 1.8 PDF

    Supported commands are:
        * Start/Stop simulation
        * Set/Get/Subscribe on simulation system variables and bus signals.

    FDX can connect either to a running CANoe application on windows or to a VN8xxx-box running in standalone mode.

    To enable FDX server, go to CANoe File -> Options -> Extensions -> XIL API & FDX Protocol
        -> Enable FDX
        -> Add FDX Description File

    The FDX description file is a file mapping signal names to protocol identifiers,
    this file can be generated using the FDXDescriptionFileParser from SVN InfotainmentAOV tests.


## For information how to set up

https://c1.confluence.cm.volvocars.biz/display/IHUA/CI+-+Lab+setup#CI-Labsetup-InstallationofVectorhardware(oldmethod)

https://c1.confluence.cm.volvocars.biz/display/IHUA/Setup+Blackbox+testing+for+flexray
