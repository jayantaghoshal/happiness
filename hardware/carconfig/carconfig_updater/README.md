Carconfig Updater
==========================

Carconfig Updater is responsible for receiving Carconfig Flexray frames and writing
them to disk. It will also set DTCs and DIDs if bad parameters has been received.

Carconfig description
---------
Carconfig or Car Central Configuration is a Flexray message broadcast by the SEM ECU on the Flexray bus. The message contains configuration parameters central for the entire car, for example vehicle Type, engine type etc. Since all parameters can not be contained in one message, the message payload is cycled thru so all parameters can be sent. Currently Carconfig contains 504 parameter sent in 72 frames but this can be expanded in the future. According to requirements a ECU should be able to receive all frames in 30 seconds.

IHU Carconfig
---------
The main responsibility for Carconfig Updater is to receive Carconfig frames, check them if they are valid and then store them to disk. If the Carconfig Parameters on disk has been changed the service should restart the IHU. If bad parameters has been received the service should set Diagnostics Trouble Codes (DTC) and store a Diagnostic Identifier message (DID) with the 10 first parameters. The IHU is also required to act as a bridge to the MOST Amplifier ECU and is supposed to relay unaltered Carconfig frames to that IHU.

Carconfig updater design
---------
The Service uses the Data Elements library to receive the VehCfgPrm Flexray frame, the data is received by declaring a lambda function which a back end service calls when a frame has been received. The service wait until all frames has been received or if the 30 second mark has passed. The received data is then checked against generated list (carconfig_parameterlist.h) of valid parameters and valid parameter values, if a parameter is within a valid range it's stored, but since the IHU is also required to act as a bridge to the MOST amplifier the raw value is also stored even if the received value might be bad.

If bad values were received the service connects to the diagnostics component using CommonAPI/D-Bus (ihu_diagnostics) and sets invalid unconfigured error not configured error DTCs. It also stores the 10 first received bad parameters. The diagnostics component can then request to read out those parameters. The read request is realized as a D-Bus signal and the service which the service subscribes to and then the service sends the data to the diagnostics component using a D-Bus method.

If the stored file has been updated the service will call Node State Manager using D-Bus end request a node restart.
